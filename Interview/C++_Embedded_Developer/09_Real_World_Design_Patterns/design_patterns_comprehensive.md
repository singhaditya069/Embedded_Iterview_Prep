# Real-World Design Patterns - Comprehensive Q&A

## Design Patterns for Embedded Systems

### Q1: Singleton pattern in embedded systems
**Answer:**
```c
#include <stdint.h>

// Singleton Pattern: Ensure only ONE instance of a resource
// Examples: Logger, Configuration Manager, Hardware Interface

// PROBLEM: Multiple logger instances = inconsistent output + resource waste
typedef struct {
    char buffer[1024];
    uint16_t head;
    uint16_t tail;
} Logger_t;

// SOLUTION 1: Static instance (C approach)
// ==========================================

static Logger_t *get_logger_instance(void) {
    static Logger_t logger_instance = {0};  // Created once on first call
    return &logger_instance;
}

void log_message(const char *msg) {
    Logger_t *logger = get_logger_instance();  // Always same instance
    // Add message to logger
}

void log_data(uint32_t value) {
    Logger_t *logger = get_logger_instance();  // Same instance again
    // Add data to logger
}

// SOLUTION 2: Module-level static (simpler for embedded)
// ======================================================

static Logger_t g_logger = {0};  // Global static (not truly global)

void log_init(void) {
    g_logger.head = 0;
    g_logger.tail = 0;
}

void log_printf(const char *fmt, ...) {
    // Use g_logger directly (always same instance)
}

void log_flush(void) {
    // Flush g_logger contents
}

// SOLUTION 3: Hardware resource access (realistic example)
// ========================================================

typedef struct {
    volatile uint32_t *uart_base;  // UART hardware register base
    volatile uint32_t *gpio_base;  // GPIO hardware register base
    uint32_t initialized;
} HardwareInterface_t;

static HardwareInterface_t *get_hardware_singleton(void) {
    static HardwareInterface_t hw_interface = {0};
    
    if (!hw_interface.initialized) {
        // UART0 is at 0x40004000 (STM32F4)
        hw_interface.uart_base = (volatile uint32_t *)0x40004000;
        hw_interface.gpio_base = (volatile uint32_t *)0x40020000;
        hw_interface.initialized = 1;
    }
    
    return &hw_interface;
}

void transmit_byte(uint8_t data) {
    HardwareInterface_t *hw = get_hardware_singleton();
    *(hw->uart_base + UART_DR_OFFSET) = data;
}

// Verification: Both functions access same hardware
void test_singleton_hardware(void) {
    HardwareInterface_t *hw1 = get_hardware_singleton();
    HardwareInterface_t *hw2 = get_hardware_singleton();
    
    if (hw1 == hw2) {
        printf("✓ Singleton verified: same instance\n");
    }
    
    transmit_byte('A');  // Uses hw_interface.uart_base
    transmit_byte('B');  // Uses SAME hw_interface.uart_base
}

// Verification: RAM usage
// Without singleton: Multiple Logger_t instances = wasted RAM
// With singleton: One Logger_t instance = efficient use
```

---

### Q2: Observer pattern for event handling
**Answer:**
```c
#include <stdint.h>
#include <stdio.h>

// Observer Pattern: Notify multiple objects when event occurs
// Decouples event source from event handlers

// Example: Temperature sensor notifies multiple listeners
// ├─ Display updates when temp changes
// ├─ Logger records temperature history
// ├─ Alarm triggers if temp exceeds threshold
// All without knowing about each other!

// Step 1: Define observer interface
// ==================================

typedef struct {
    uint16_t temperature;  // in 0.1°C units
} TemperatureEvent_t;

typedef void (*OnTemperatureChanged)(TemperatureEvent_t *event);

// Step 2: Create event source (Temperature Sensor)
// =================================================

#define MAX_LISTENERS 5

typedef struct {
    OnTemperatureChanged listeners[MAX_LISTENERS];
    uint8_t listener_count;
    uint16_t current_temp;
} TemperatureSensor_t;

static TemperatureSensor_t g_temp_sensor = {0};

void sensor_init(void) {
    g_temp_sensor.listener_count = 0;
    g_temp_sensor.current_temp = 0;
}

// Attach observer
void sensor_subscribe(OnTemperatureChanged handler) {
    if (g_temp_sensor.listener_count < MAX_LISTENERS) {
        g_temp_sensor.listeners[g_temp_sensor.listener_count++] = handler;
    }
}

// Detach observer
void sensor_unsubscribe(OnTemperatureChanged handler) {
    for (int i = 0; i < g_temp_sensor.listener_count; i++) {
        if (g_temp_sensor.listeners[i] == handler) {
            // Remove by shifting
            for (int j = i; j < g_temp_sensor.listener_count - 1; j++) {
                g_temp_sensor.listeners[j] = g_temp_sensor.listeners[j + 1];
            }
            g_temp_sensor.listener_count--;
            break;
        }
    }
}

// Main event trigger (notify all observers)
void sensor_set_temperature(uint16_t temp) {
    g_temp_sensor.current_temp = temp;
    
    // Notify ALL listeners
    TemperatureEvent_t event = {.temperature = temp};
    
    for (int i = 0; i < g_temp_sensor.listener_count; i++) {
        if (g_temp_sensor.listeners[i]) {
            g_temp_sensor.listeners[i](&event);  // Call observer
        }
    }
}

// Step 3: Define observers
// =========================

// Observer 1: Display
void on_temp_display_handler(TemperatureEvent_t *event) {
    printf("DISPLAY: Temperature = %.1f°C\n", 
           event->temperature / 10.0f);
}

// Observer 2: Logger
void on_temp_logger_handler(TemperatureEvent_t *event) {
    static uint32_t sample_count = 0;
    printf("LOG: Sample #%lu, Temp = %.1f°C\n", 
           sample_count++, event->temperature / 10.0f);
}

// Observer 3: Alarm
void on_temp_alarm_handler(TemperatureEvent_t *event) {
    if (event->temperature > 400) {  // 40.0°C threshold
        printf("ALARM: Temperature too high: %.1f°C!\n", 
               event->temperature / 10.0f);
    }
}

// Step 4: Usage
// =============

void test_observer_pattern(void) {
    sensor_init();
    
    // Subscribe observers
    sensor_subscribe(on_temp_display_handler);
    sensor_subscribe(on_temp_logger_handler);
    sensor_subscribe(on_temp_alarm_handler);
    
    printf("=== Sensor notifies all observers ===\n");
    sensor_set_temperature(250);  // 25.0°C
    
    printf("\n=== Temperature rises ===\n");
    sensor_set_temperature(450);  // 45.0°C (triggers alarm!)
    
    printf("\n=== Unsubscribe logger ===\n");
    sensor_unsubscribe(on_temp_logger_handler);
    
    printf("\n=== Only display and alarm get events ===\n");
    sensor_set_temperature(380);  // 38.0°C
}

// Output:
// === Sensor notifies all observers ===
// DISPLAY: Temperature = 25.0°C
// LOG: Sample #0, Temp = 25.0°C
//
// === Temperature rises ===
// DISPLAY: Temperature = 45.0°C
// LOG: Sample #1, Temp = 45.0°C
// ALARM: Temperature too high: 45.0°C!
//
// === Unsubscribe logger ===
//
// === Only display and alarm get events ===
// DISPLAY: Temperature = 38.0°C
// ALARM: Temperature too high: 45.0°C!  (different threshold check)

// Key benefits:
// ✓ Loosely coupled (observers don't know each other)
// ✓ Easy to add/remove observers
// ✓ Scales to many listeners
// ✗ Can be wasteful if too many observers
```

---

### Q3: State machine pattern for device control
**Answer:**
```c
#include <stdint.h>
#include <stdio.h>

// State Machine Pattern: Device behavior changes based on state
// Clean, predictable, maintainable

// Example: Smart Light Controller
// States: OFF → DIMMED → BRIGHT
// Transitions based on button presses and timers

// State enum
typedef enum {
    LIGHT_STATE_OFF = 0,
    LIGHT_STATE_DIMMED = 1,
    LIGHT_STATE_BRIGHT = 2,
    LIGHT_STATE_COUNT
} LightState_t;

// Events
typedef enum {
    LIGHT_EVENT_BUTTON_PRESS = 0,
    LIGHT_EVENT_TIMER_EXPIRED = 1,
    LIGHT_EVENT_AUTO_OFF = 2,
    LIGHT_EVENT_COUNT
} LightEvent_t;

// Context: Data for current state
typedef struct {
    LightState_t state;
    uint32_t brightness;  // 0-100%
    uint32_t auto_off_ms; // Auto-off timer
} LightContext_t;

static LightContext_t g_light_ctx = {0};

// Action functions (what happens in each state)
// ==============================================

static void light_enter_off_state(void) {
    printf("LIGHT: Entering OFF state\n");
    g_light_ctx.brightness = 0;
}

static void light_exit_off_state(void) {
    printf("LIGHT: Exiting OFF state\n");
}

static void light_enter_dimmed_state(void) {
    printf("LIGHT: Entering DIMMED state (50%% brightness)\n");
    g_light_ctx.brightness = 50;
}

static void light_enter_bright_state(void) {
    printf("LIGHT: Entering BRIGHT state (100%% brightness)\n");
    g_light_ctx.brightness = 100;
}

// State function pointers
typedef struct {
    void (*on_enter)(void);  // Called when entering state
    void (*on_exit)(void);   // Called when exiting state
} StateHandler_t;

static const StateHandler_t state_handlers[] = {
    [LIGHT_STATE_OFF] = {
        .on_enter = light_enter_off_state,
        .on_exit = light_exit_off_state,
    },
    [LIGHT_STATE_DIMMED] = {
        .on_enter = light_enter_dimmed_state,
        .on_exit = NULL,
    },
    [LIGHT_STATE_BRIGHT] = {
        .on_enter = light_enter_bright_state,
        .on_exit = NULL,
    },
};

// State transition table
// ====================================================================
// |    CURRENT STATE | EVENT (Button) | EVENT (Timer) | EVENT (Auto) |
// |    ─────────── | ──────────────── | ──────────── | ──────────── |
// | OFF            | → DIMMED         | STAY OFF     | STAY OFF     |
// | DIMMED         | → BRIGHT         | STAY DIMMED  | → OFF        |
// | BRIGHT         | → OFF            | STAY BRIGHT  | → OFF        |
// ====================================================================

typedef enum {
    TRANSITION_STAY,     // No state change
    TRANSITION_CHANGE,   // Change to new state
} TransitionType_t;

typedef struct {
    LightState_t next_state;
    TransitionType_t type;
} Transition_t;

static const Transition_t state_transitions[LIGHT_STATE_COUNT][LIGHT_EVENT_COUNT] = {
    // OFF state
    [LIGHT_STATE_OFF] = {
        [LIGHT_EVENT_BUTTON_PRESS] = {LIGHT_STATE_DIMMED, TRANSITION_CHANGE},
        [LIGHT_EVENT_TIMER_EXPIRED] = {LIGHT_STATE_OFF, TRANSITION_STAY},
        [LIGHT_EVENT_AUTO_OFF] = {LIGHT_STATE_OFF, TRANSITION_STAY},
    },
    
    // DIMMED state
    [LIGHT_STATE_DIMMED] = {
        [LIGHT_EVENT_BUTTON_PRESS] = {LIGHT_STATE_BRIGHT, TRANSITION_CHANGE},
        [LIGHT_EVENT_TIMER_EXPIRED] = {LIGHT_STATE_DIMMED, TRANSITION_STAY},
        [LIGHT_EVENT_AUTO_OFF] = {LIGHT_STATE_OFF, TRANSITION_CHANGE},
    },
    
    // BRIGHT state
    [LIGHT_STATE_BRIGHT] = {
        [LIGHT_EVENT_BUTTON_PRESS] = {LIGHT_STATE_OFF, TRANSITION_CHANGE},
        [LIGHT_EVENT_TIMER_EXPIRED] = {LIGHT_STATE_BRIGHT, TRANSITION_STAY},
        [LIGHT_EVENT_AUTO_OFF] = {LIGHT_STATE_OFF, TRANSITION_CHANGE},
    },
};

// Handle event (main state machine logic)
void light_handle_event(LightEvent_t event) {
    printf("EVENT: %d in state %d\n", event, g_light_ctx.state);
    
    Transition_t trans = state_transitions[g_light_ctx.state][event];
    
    if (trans.type == TRANSITION_CHANGE) {
        // Call exit handler of current state
        if (state_handlers[g_light_ctx.state].on_exit) {
            state_handlers[g_light_ctx.state].on_exit();
        }
        
        // Change state
        g_light_ctx.state = trans.next_state;
        
        // Call enter handler of new state
        if (state_handlers[g_light_ctx.state].on_enter) {
            state_handlers[g_light_ctx.state].on_enter();
        }
    } else {
        printf("LIGHT: Staying in state %d\n", g_light_ctx.state);
    }
}

// Initialization
void light_init(void) {
    g_light_ctx.state = LIGHT_STATE_OFF;
    g_light_ctx.brightness = 0;
    light_enter_off_state();
}

// Test
void test_state_machine(void) {
    light_init();
    
    printf("=== Initial State ===\n");
    printf("State: %d, Brightness: %d%%\n\n", g_light_ctx.state, g_light_ctx.brightness);
    
    printf("=== Press Button (OFF → DIMMED) ===\n");
    light_handle_event(LIGHT_EVENT_BUTTON_PRESS);
    printf("State: %d, Brightness: %d%%\n\n", g_light_ctx.state, g_light_ctx.brightness);
    
    printf("=== Press Button Again (DIMMED → BRIGHT) ===\n");
    light_handle_event(LIGHT_EVENT_BUTTON_PRESS);
    printf("State: %d, Brightness: %d%%\n\n", g_light_ctx.state, g_light_ctx.brightness);
    
    printf("=== Auto-off triggered (BRIGHT → OFF) ===\n");
    light_handle_event(LIGHT_EVENT_AUTO_OFF);
    printf("State: %d, Brightness: %d%%\n\n", g_light_ctx.state, g_light_ctx.brightness);
}

// Output:
// === Initial State ===
// LIGHT: Entering OFF state
// State: 0, Brightness: 0%
//
// === Press Button (OFF → DIMMED) ===
// EVENT: 0 in state 0
// LIGHT: Exiting OFF state
// LIGHT: Entering DIMMED state (50% brightness)
// State: 1, Brightness: 50%
//
// === Press Button Again (DIMMED → BRIGHT) ===
// EVENT: 0 in state 1
// LIGHT: Entering BRIGHT state (100% brightness)
// State: 2, Brightness: 100%
//
// === Auto-off triggered (BRIGHT → OFF) ===
// EVENT: 2 in state 2
// LIGHT: Exiting ...
// LIGHT: Entering OFF state
// State: 0, Brightness: 0%

// Benefits:
// ✓ Clear state transitions
// ✓ Easy to visualize (state diagram)
// ✓ Handles all cases explicitly
// ✓ Prevents invalid states
// ✓ Scales to complex systems
```

---

### Q4: Factory pattern for object creation
**Answer:**
```c
#include <stdint.h>

// Factory Pattern: Create objects without specifying exact classes
// Centralized creation logic

// Example: Multiple LED types, different control methods

// Abstract interface
typedef struct {
    void (*init)(void);
    void (*turn_on)(void);
    void (*turn_off)(void);
    void (*set_brightness)(uint8_t percent);
} LED_Interface_t;

// LED Type 1: SimpleLED (digital on/off)
// ======================================

typedef struct {
    uint32_t gpio_port;
    uint8_t gpio_pin;
} SimpleLED_t;

static SimpleLED_t simple_led_instance = {
    .gpio_port = GPIOA_BASE,
    .gpio_pin = 3,
};

void simple_led_init(void) {
    printf("SimpleLED: Init on GPIO pin 3\n");
}

void simple_led_on(void) {
    printf("SimpleLED: ON (full brightness)\n");
}

void simple_led_off(void) {
    printf("SimpleLED: OFF\n");
}

void simple_led_set_brightness(uint8_t percent) {
    if (percent > 0) {
        simple_led_on();
    } else {
        simple_led_off();
    }
}

// LED Type 2: PWMLED (variable brightness via PWM)
// ================================================

typedef struct {
    uint32_t timer_base;
    uint8_t pwm_channel;
    uint8_t current_brightness;
} PWMLED_t;

static PWMLED_t pwm_led_instance = {
    .timer_base = TIM3_BASE,
    .pwm_channel = 1,
    .current_brightness = 0,
};

void pwm_led_init(void) {
    printf("PWMLED: Init on PWM timer 3, channel 1\n");
}

void pwm_led_on(void) {
    // Set PWM to 100%
    pwm_led_set_brightness(100);
}

void pwm_led_off(void) {
    // Set PWM to 0%
    pwm_led_set_brightness(0);
}

void pwm_led_set_brightness(uint8_t percent) {
    pwm_led_instance.current_brightness = percent;
    printf("PWMLED: Brightness = %d%% (PWM duty cycle)\n", percent);
}

// LED Type 3: RgbLED (color and brightness)
// ==========================================

typedef struct {
    uint8_t red_brightness;
    uint8_t green_brightness;
    uint8_t blue_brightness;
} RgbLED_t;

static RgbLED_t rgb_led_instance = {0};

void rgb_led_init(void) {
    printf("RgbLED: Init RGB channels\n");
}

void rgb_led_on(void) {
    rgb_led_set_brightness(100);
}

void rgb_led_off(void) {
    rgb_led_set_brightness(0);
}

void rgb_led_set_brightness(uint8_t percent) {
    rgb_led_instance.red_brightness = percent;
    rgb_led_instance.green_brightness = percent;
    rgb_led_instance.blue_brightness = percent;
    printf("RgbLED: Brightness = %d%% (all channels)\n", percent);
}

// Factory: Create LED instances
// ==============================

typedef enum {
    LED_TYPE_SIMPLE = 0,
    LED_TYPE_PWM = 1,
    LED_TYPE_RGB = 2,
} LED_Type_t;

static LED_Interface_t simple_led_interface = {
    .init = simple_led_init,
    .turn_on = simple_led_on,
    .turn_off = simple_led_off,
    .set_brightness = simple_led_set_brightness,
};

static LED_Interface_t pwm_led_interface = {
    .init = pwm_led_init,
    .turn_on = pwm_led_on,
    .turn_off = pwm_led_off,
    .set_brightness = pwm_led_set_brightness,
};

static LED_Interface_t rgb_led_interface = {
    .init = rgb_led_init,
    .turn_on = rgb_led_on,
    .turn_off = rgb_led_off,
    .set_brightness = rgb_led_set_brightness,
};

// FACTORY FUNCTION
LED_Interface_t *create_led(LED_Type_t type) {
    LED_Interface_t *led = NULL;
    
    switch (type) {
        case LED_TYPE_SIMPLE:
            led = &simple_led_interface;
            printf("FACTORY: Created SimpleLED\n");
            break;
            
        case LED_TYPE_PWM:
            led = &pwm_led_interface;
            printf("FACTORY: Created PWMLED\n");
            break;
            
        case LED_TYPE_RGB:
            led = &rgb_led_interface;
            printf("FACTORY: Created RgbLED\n");
            break;
            
        default:
            printf("FACTORY: Unknown LED type %d\n", type);
    }
    
    return led;
}

// Usage (application code doesn't know LED type!)
// =================================================

void test_factory_pattern(void) {
    // Caller doesn't need to know implementation details!
    
    LED_Interface_t *status_led = create_led(LED_TYPE_PWM);
    LED_Interface_t *power_led = create_led(LED_TYPE_SIMPLE);
    LED_Interface_t *rgb_led = create_led(LED_TYPE_RGB);
    
    printf("\n=== Initialize all LEDs ===\n");
    status_led->init();
    power_led->init();
    rgb_led->init();
    
    printf("\n=== Turn on ===\n");
    status_led->turn_on();
    power_led->turn_on();
    rgb_led->turn_on();
    
    printf("\n=== Set brightness ===\n");
    status_led->set_brightness(75);
    power_led->set_brightness(50);
    rgb_led->set_brightness(100);
    
    printf("\n=== Turn off ===\n");
    status_led->turn_off();
    power_led->turn_off();
    rgb_led->turn_off();
}

// Benefits:
// ✓ Single place to configure LED types
// ✓ Easy to swap LED implementations
// ✓ Uniform interface for all LED types
// ✓ Minimal code changes when adding new LED type
```

Design patterns make embedded systems more maintainable and scalable!
