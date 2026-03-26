# System Design Architecture - Comprehensive Q&A

## System Architecture Patterns

### Q1: Layered architecture for embedded systems
**Answer:**
```c
// Typical embedded system layered architecture

// ┌─────────────────────────────────┐
// │   Application Layer             │ (User logic, state machines)
// └─────────────────────────────────┘
//            │
// ┌─────────────────────────────────┐
// │  Framework/Services Layer       │ (RTOS, logging, config)
// └─────────────────────────────────┘
//            │
// ┌─────────────────────────────────┐
// │   HAL/Driver Layer              │ (GPIO, UART, SPI, etc.)
// └─────────────────────────────────┘
//            │
// ┌─────────────────────────────────┐
// │   Hardware Layer                │ (MCU, peripherals)
// └─────────────────────────────────┘

// Implementation example

// ---- Hardware Abstraction Layer ----
#include <stdint.h>

typedef struct {
    volatile uint32_t *gpio_port;
    uint8_t pin;
} GPIO_Handle_t;

// HAL function - hardware independent interface
void hal_gpio_write(GPIO_Handle_t *handle, uint8_t value) {
    if (value) {
        handle->gpio_port[0] |= (1 << handle->pin);   // Set high
    } else {
        handle->gpio_port[0] &= ~(1 << handle->pin);  // Set low
    }
}

uint8_t hal_gpio_read(GPIO_Handle_t *handle) {
    return (uint8_t)((handle->gpio_port[1] >> handle->pin) & 1);
}

// ---- Driver Layer ----
typedef struct {
    GPIO_Handle_t led_pin;
    GPIO_Handle_t button_pin;
} LedDriver_t;

void led_driver_init(LedDriver_t *driver, 
                     volatile uint32_t *gpio_base) {
    driver->led_pin.gpio_port = gpio_base;
    driver->led_pin.pin = 5;
    
    driver->button_pin.gpio_port = gpio_base;
    driver->button_pin.pin = 3;
}

void led_driver_on(LedDriver_t *driver) {
    hal_gpio_write(&driver->led_pin, 1);
}

void led_driver_off(LedDriver_t *driver) {
    hal_gpio_write(&driver->led_pin, 0);
}

uint8_t led_driver_button_pressed(LedDriver_t *driver) {
    return !hal_gpio_read(&driver->button_pin);  // Active low
}

// ---- Application Layer ----
#include <stdbool.h>

typedef struct {
    LedDriver_t driver;
    uint32_t blink_delay;
    bool led_on;
} LedApplication_t;

void app_init(LedApplication_t *app, volatile uint32_t *gpio_base) {
    led_driver_init(&app->driver, gpio_base);
    app->blink_delay = 500;  // 500ms
    app->led_on = false;
}

void app_update(LedApplication_t *app) {
    // Button check
    if (led_driver_button_pressed(&app->driver)) {
        app->led_on = !app->led_on;  // Toggle
    }
    
    // LED control
    if (app->led_on) {
        led_driver_on(&app->driver);
    } else {
        led_driver_off(&app->driver);
    }
}

// Usage
int main(void) {
    LedApplication_t app;
    app_init(&app, (volatile uint32_t *)0x40000000);
    
    while (1) {
        app_update(&app);
    }
    
    return 0;
}

// Benefits of layering:
// ✓ Hardware independence - change MCU without changing application
// ✓ Testability - mock HAL for unit testing
// ✓ Code reuse - same driver works on multiple products
// ✓ Maintainability - clear separation of concerns
// ✓ Scalability - easy to add new features
```

---

### Q2: Model-View-Controller (MVC) in embedded
**Answer:**
```c
#include <stdint.h>
#include <string.h>

// ---- MODEL - Data and state management ----
typedef struct {
    uint16_t current_temperature;
    uint16_t target_temperature;
    uint16_t min_temperature;
    uint16_t max_temperature;
    uint8_t heating_on;
    uint32_t runtime_seconds;
} ThermostatModel_t;

void model_init(ThermostatModel_t *model) {
    model->current_temperature = 20;
    model->target_temperature = 22;
    model->min_temperature = 15;
    model->max_temperature = 30;
    model->heating_on = 0;
    model->runtime_seconds = 0;
}

void model_set_temperature(ThermostatModel_t *model, uint16_t target) {
    if (target >= model->min_temperature && 
        target <= model->max_temperature) {
        model->target_temperature = target;
    }
}

void model_update_current(ThermostatModel_t *model, uint16_t current) {
    model->current_temperature = current;
}

uint8_t model_should_heat(ThermostatModel_t *model) {
    // Logic: heat if below target, 1°C hysteresis
    if (model->current_temperature < 
        (model->target_temperature - 1)) {
        return 1;  // Turn heater on
    }
    if (model->current_temperature >= 
        model->target_temperature) {
        return 0;  // Turn heater off
    }
    return model->heating_on;  // Keep current state
}

// ---- VIEW - Display and UI ----
typedef struct {
    char display_buffer[32];
    uint8_t display_updated;
} ThermostatView_t;

void view_init(ThermostatView_t *view) {
    memset(view->display_buffer, 0, sizeof(view->display_buffer));
    view->display_updated = 0;
}

void view_update(ThermostatView_t *view, 
                const ThermostatModel_t *model) {
    // Format temperature display
    snprintf(view->display_buffer, sizeof(view->display_buffer),
            "T: %d°C  Target: %d°C  %s",
            model->current_temperature,
            model->target_temperature,
            model->heating_on ? "HEATING" : "OFF");
    
    view->display_updated = 1;
}

void view_render(ThermostatView_t *view) {
    if (view->display_updated) {
        // Send to display device (LCD, OLED, etc.)
        lcd_display_string(view->display_buffer);
        view->display_updated = 0;
    }
}

// ---- CONTROLLER - Input and logic ----
typedef struct {
    ThermostatModel_t *model;
    ThermostatView_t *view;
} ThermostatController_t;

void controller_init(ThermostatController_t *ctrl,
                    ThermostatModel_t *model,
                    ThermostatView_t *view) {
    ctrl->model = model;
    ctrl->view = view;
}

void controller_handle_button_up(ThermostatController_t *ctrl) {
    uint16_t new_temp = ctrl->model->target_temperature + 1;
    model_set_temperature(ctrl->model, new_temp);
    view_update(ctrl->view, ctrl->model);
}

void controller_handle_button_down(ThermostatController_t *ctrl) {
    uint16_t new_temp = ctrl->model->target_temperature - 1;
    model_set_temperature(ctrl->model, new_temp);
    view_update(ctrl->view, ctrl->model);
}

void controller_update(ThermostatController_t *ctrl,
                      uint16_t current_temp) {
    model_update_current(ctrl->model, current_temp);
    
    uint8_t should_heat = model_should_heat(ctrl->model);
    if (should_heat != ctrl->model->heating_on) {
        ctrl->model->heating_on = should_heat;
        if (should_heat) {
            heater_on();
        } else {
            heater_off();
        }
    }
    
    view_update(ctrl->view, ctrl->model);
}

// Usage
typedef struct {
    ThermostatModel_t model;
    ThermostatView_t view;
    ThermostatController_t controller;
} Thermostat_t;

int main(void) {
    Thermostat_t thermostat;
    
    model_init(&thermostat.model);
    view_init(&thermostat.view);
    controller_init(&thermostat.controller, 
                   &thermostat.model,
                   &thermostat.view);
    
    while (1) {
        // Read temperature from sensor
        uint16_t current_temp = adc_read_temperature();
        controller_update(&thermostat.controller, current_temp);
        
        // Check buttons
        if (button_up_pressed()) {
            controller_handle_button_up(&thermostat.controller);
        }
        if (button_down_pressed()) {
            controller_handle_button_down(&thermostat.controller);
        }
        
        // Render display
        view_render(&thermostat.view);
    }
    
    return 0;
}

// MVC Benefits:
// Model - Contains all logic, independent of hardware
// View - Formats data for display, no processing
// Controller - Handles input and coordinates Model/View
// Advantage: Can change display (LCD→OLED) without changing model
```

---

## Design Patterns in Embedded

### Q1: State machine pattern
**Answer:**
```c
#include <stdint.h>

// State machine for wireless communications

typedef enum {
    STATE_IDLE,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_TRANSMITTING,
    STATE_ERROR,
    STATE_MAX
} WifiState_t;

typedef enum {
    EVENT_CONNECT,
    EVENT_CONNECTED,
    EVENT_DISCONNECT,
    EVENT_SEND_DATA,
    EVENT_DATA_SENT,
    EVENT_ERROR,
    EVENT_TIMEOUT,
    EVENT_MAX
} WifiEvent_t;

typedef struct {
    WifiState_t current_state;
    uint32_t state_enter_time;
    uint32_t timeout_ms;
} WifiStateMachine_t;

uint32_t get_ticks(void);

// State handler functions
void state_idle_enter(WifiStateMachine_t *fsm) {
    printf("State: IDLE\n");
    fsm->state_enter_time = get_ticks();
}

void state_idle_handle(WifiStateMachine_t *fsm, WifiEvent_t event) {
    switch (event) {
        case EVENT_CONNECT:
            printf("Connecting...\n");
            wifi_connect_start();
            fsm->current_state = STATE_CONNECTING;
            fsm->timeout_ms = 5000;  // 5 second timeout
            break;
            
        default:
            break;
    }
}

void state_connecting_enter(WifiStateMachine_t *fsm) {
    printf("State: CONNECTING\n");
    fsm->state_enter_time = get_ticks();
}

void state_connecting_handle(WifiStateMachine_t *fsm, WifiEvent_t event) {
    uint32_t elapsed = get_ticks() - fsm->state_enter_time;
    
    if (elapsed > fsm->timeout_ms) {
        printf("Connection timeout!\n");
        fsm->current_state = STATE_ERROR;
        return;
    }
    
    switch (event) {
        case EVENT_CONNECTED:
            printf("Connected!\n");
            fsm->current_state = STATE_CONNECTED;
            break;
            
        case EVENT_ERROR:
            printf("Connection failed!\n");
            fsm->current_state = STATE_ERROR;
            break;
            
        default:
            break;
    }
}

void state_connected_enter(WifiStateMachine_t *fsm) {
    printf("State: CONNECTED\n");
    fsm->state_enter_time = get_ticks();
}

void state_connected_handle(WifiStateMachine_t *fsm, WifiEvent_t event) {
    switch (event) {
        case EVENT_SEND_DATA:
            printf("Transmitting...\n");
            wifi_send_data_start();
            fsm->current_state = STATE_TRANSMITTING;
            fsm->timeout_ms = 2000;
            break;
            
        case EVENT_DISCONNECT:
            printf("Disconnecting...\n");
            fsm->current_state = STATE_IDLE;
            break;
            
        default:
            break;
    }
}

void state_transmitting_enter(WifiStateMachine_t *fsm) {
    printf("State: TRANSMITTING\n");
    fsm->state_enter_time = get_ticks();
}

void state_transmitting_handle(WifiStateMachine_t *fsm, WifiEvent_t event) {
    uint32_t elapsed = get_ticks() - fsm->state_enter_time;
    
    if (elapsed > fsm->timeout_ms) {
        printf("Transmission timeout!\n");
        fsm->current_state = STATE_ERROR;
        return;
    }
    
    switch (event) {
        case EVENT_DATA_SENT:
            printf("Data sent!\n");
            fsm->current_state = STATE_CONNECTED;
            break;
            
        case EVENT_ERROR:
            printf("Transmission failed!\n");
            fsm->current_state = STATE_ERROR;
            break;
            
        default:
            break;
    }
}

void state_error_enter(WifiStateMachine_t *fsm) {
    printf("State: ERROR\n");
    fsm->state_enter_time = get_ticks();
}

void state_error_handle(WifiStateMachine_t *fsm, WifiEvent_t event) {
    uint32_t elapsed = get_ticks() - fsm->state_enter_time;
    
    if (elapsed > 3000) {  // 3 second recovery timeout
        printf("Recovering from error...\n");
        fsm->current_state = STATE_IDLE;
    }
}

// State transition table
typedef void (*StateEnterFunc)(WifiStateMachine_t *);
typedef void (*StateHandleFunc)(WifiStateMachine_t *, WifiEvent_t);

typedef struct {
    StateEnterFunc enter;
    StateHandleFunc handle;
} StateHandler_t;

const StateHandler_t state_handlers[STATE_MAX] = {
    [STATE_IDLE] = {state_idle_enter, state_idle_handle},
    [STATE_CONNECTING] = {state_connecting_enter, state_connecting_handle},
    [STATE_CONNECTED] = {state_connected_enter, state_connected_handle},
    [STATE_TRANSMITTING] = {state_transmitting_enter, state_transmitting_handle},
    [STATE_ERROR] = {state_error_enter, state_error_handle},
};

// FSM initialization
WifiStateMachine_t fsm;
WifiState_t last_state = STATE_MAX;

void fsm_init(void) {
    fsm.current_state = STATE_IDLE;
    fsm.state_enter_time = get_ticks();
    fsm.timeout_ms = 0;
    state_handlers[STATE_IDLE].enter(&fsm);
}

// Main FSM handler
void fsm_handle_event(WifiEvent_t event) {
    // Call exit handler if state changed
    if (fsm.current_state != last_state) {
        state_handlers[fsm.current_state].enter(&fsm);
        last_state = fsm.current_state;
    }
    
    // Handle current state
    if (state_handlers[fsm.current_state].handle) {
        state_handlers[fsm.current_state].handle(&fsm, event);
    }
}

// Usage
int main(void) {
    fsm_init();
    
    while (1) {
        // Check WiFi events
        WifiEvent_t event = get_wifi_event();
        if (event != EVENT_MAX) {
            fsm_handle_event(event);
        }
    }
    
    return 0;
}

// State machine benefits:
// ✓ Clear state transitions
// ✓ Robust error handling
// ✓ Easy to test
// ✓ Handles edge cases
// ✓ More maintainable than spaghetti code
```

---

### Q2: Observer pattern for system events
**Answer:**
```c
#include <stdint.h>
#include <string.h>

// System event types
typedef enum {
    EVENT_TEMPERATURE_ALERT,
    EVENT_BATTERY_LOW,
    EVENT_SENSOR_ERROR,
    EVENT_CONNECTION_LOST,
    EVENT_MAX
} SystemEvent_t;

// Observer callback type
typedef void (*EventObserver_t)(SystemEvent_t event, uint32_t data);

// Observer registry
#define MAX_OBSERVERS 10

typedef struct {
    EventObserver_t observers[MAX_OBSERVERS];
    uint8_t count;
} EventManager_t;

EventManager_t event_manager;

// Subscribe to event
void event_subscribe(EventObserver_t observer) {
    if (event_manager.count < MAX_OBSERVERS) {
        event_manager.observers[event_manager.count++] = observer;
    }
}

// Publish event to all observerss
void event_publish(SystemEvent_t event, uint32_t data) {
    for (uint8_t i = 0; i < event_manager.count; i++) {
        if (event_manager.observers[i]) {
            event_manager.observers[i](event, data);
        }
    }
}

// Observer implementations
void logger_observer(SystemEvent_t event, uint32_t data) {
    const char *event_names[] = {
        "TEMP_ALERT", "BATTERY_LOW", "SENSOR_ERROR", "CONN_LOST"
    };
    printf("[LOG] Event: %s, Data: %u\n", event_names[event], data);
}

void display_observer(SystemEvent_t event, uint32_t data) {
    switch (event) {
        case EVENT_TEMPERATURE_ALERT:
            lcd_display_warning("TEMP: %u°C", data);
            break;
        case EVENT_BATTERY_LOW:
            lcd_display_warning("BATTERY: %u%%", data);
            break;
        case EVENT_SENSOR_ERROR:
            lcd_display_warning("SENSOR ERR");
            break;
        default:
            break;
    }
}

void audio_observer(SystemEvent_t event, uint32_t data) {
    if (event == EVENT_TEMPERATURE_ALERT) {
        buzzer_beep_sequence(3);  // Alert beeping
    }
}

void storage_observer(SystemEvent_t event, uint32_t data) {
    // Log to flash storage
    event_log_store(event, data);
}

// Usage
int main(void) {
    // Register observers
    event_subscribe(logger_observer);
    event_subscribe(display_observer);
    event_subscribe(audio_observer);
    event_subscribe(storage_observer);
    
    // Simulate events
    event_publish(EVENT_TEMPERATURE_ALERT, 42);  // 42°C
    event_publish(EVENT_BATTERY_LOW, 15);        // 15%
    
    return 0;
}

// Observer pattern benefits:
// ✓ Loose coupling - observers don't know about each other
// ✓ Easy to add/remove observers
// ✓ Multiple reactions to same event
// ✓ Good for system-wide events
```

System design architecture ensures robust, maintainable embedded software!
