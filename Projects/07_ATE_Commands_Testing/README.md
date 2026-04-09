# ATE Commands & Testing - Complete Implementation Guide

## Project Overview

Automated Test Equipment (ATE) is essential for production quality assurance. This project covers ATE command implementation and UNITY testing framework integration.

---

## ATE Architecture

### ATE Command Protocol

```
┌────────────────────────────────────────┐
│   Test Controller (PC/Server)          │
│   • Test planning                      │
│   • Result logging                     │
│   • Report generation                  │
└────────┬─────────────────────────────┘
         │ Serial/Ethernet
         ▼
┌────────────────────────────────────────┐
│   Device Under Test (DUT)              │
│   • Executes ATE commands              │
│   • Returns test results               │
│   • Manages test state                 │
└────────────────────────────────────────┘
```

### Command Structure

```
Format: <TYPE:CMD:PARAM1:PARAM2:...>
Example: <TEST:GPIO:LED1:ON>
         <MEAS:ADC:CH0>
         <SET:UART:115200>
```

### ATE Command Handler

```c
// ate_commands.h
#ifndef ATE_COMMANDS_H
#define ATE_COMMANDS_H

typedef enum {
    ATE_CMD_SET = 0,      // Configuration commands
    ATE_CMD_GET,          // Read/measure commands
    ATE_CMD_TEST,         // Test execution
    ATE_CMD_RESULT,       // Result retrieval
    ATE_CMD_STATUS        // System status
} ate_cmd_type_t;

typedef struct {
    char command[128];
    char response[256];
    uint32_t timeout_ms;
} ate_command_t;

int ATE_ProcessCommand(const char *cmd, char *response, size_t resp_len);
int ATE_GPIO_Test(const char *pin, const char *action);
int ATE_ADC_Measure(uint8_t channel, uint16_t *value);
int ATE_UART_Loopback(void);
int ATE_I2C_Scan(void);

#endif
```

### Implementation

```c
#include "ate_commands.h"

int ATE_ProcessCommand(const char *cmd, char *response, size_t resp_len)
{
    char cmd_type[32], cmd_name[32];
    int args[4] = {0};
    
    // Parse command: <TYPE:CMD:ARG1:ARG2:ARG3>
    int parsed = sscanf(cmd, "<%[^:]:%[^:]:%d:%d:%d:%d>",
                        cmd_type, cmd_name, 
                        &args[0], &args[1], &args[2], &args[3]);
    
    if (parsed < 2) {
        snprintf(response, resp_len, "<ERROR:PARSE>");
        return -1;
    }
    
    // Route to appropriate handler
    if (strcmp(cmd_type, "TEST") == 0) {
        return ATE_HandleTest(cmd_name, args, response, resp_len);
    }
    else if (strcmp(cmd_type, "SET") == 0) {
        return ATE_HandleSet(cmd_name, args, response, resp_len);
    }
    else if (strcmp(cmd_type, "GET") == 0) {
        return ATE_HandleGet(cmd_name, args, response, resp_len);
    }
    
    snprintf(response, resp_len, "<ERROR:UNKNOWN>");
    return -1;
}

int ATE_GPIO_Test(const char *pin, const char *action)
{
    if (strcmp(action, "ON") == 0) {
        GPIO_WritePin(pin, 1);
    } 
    else if (strcmp(action, "OFF") == 0) {
        GPIO_WritePin(pin, 0);
    }
    else if (strcmp(action, "TOGGLE") == 0) {
        GPIO_TogglePin(pin);
    }
    else if (strcmp(action, "READ") == 0) {
        return GPIO_ReadPin(pin);
    }
    return 0;
}

int ATE_ADC_Measure(uint8_t channel, uint16_t *value)
{
    ADC_StartConversion(channel);
    *value = ADC_GetResult();
    return 0;
}

int ATE_UART_Loopback(void)
{
    uint8_t test_data[] = {0xAA, 0x55, 0xFF, 0x00};
    uint8_t receive_data[4];
    
    // Send data
    UART_Transmit(test_data, 4);
    
    // Receive echo
    if (UART_Receive(receive_data, 4, 1000) != 4)
        return -1;
    
    // Compare
    for (int i = 0; i < 4; i++) {
        if (test_data[i] != receive_data[i])
            return -1;
    }
    
    return 0;
}

int ATE_I2C_Scan(void)
{
    int device_count = 0;
    
    for (int addr = 0x08; addr < 0x78; addr++) {
        if (I2C_Ping(addr) == 0) {
            device_count++;
            // Log found device
        }
    }
    
    return device_count;
}
```

---

## UNITY Testing Framework

### UNITY Basics

```c
// test_sensor.c
#include "unity.h"
#include "sensor.h"

// Setup and teardown
void setUp(void)
{
    Sensor_Init();
}

void tearDown(void)
{
    Sensor_DeInit();
}

// Test cases
void test_Sensor_ReadValidValue(void)
{
    uint16_t value = Sensor_GetValue();
    TEST_ASSERT_GREATER_THAN(0, value);
    TEST_ASSERT_LESS_THAN(4096, value);
}

void test_Sensor_ReadTwice_SameValue(void)
{
    uint16_t val1 = Sensor_GetValue();
    uint16_t val2 = Sensor_GetValue();
    TEST_ASSERT_EQUAL(val1, val2);
}

void test_Sensor_ThresholdDetection(void)
{
    Sensor_SetThreshold(2000);
    bool detected = Sensor_IsAboveThreshold();
    TEST_ASSERT_FALSE(detected);
    
    // Simulate value change
    Sensor_SetValue(3000);
    detected = Sensor_IsAboveThreshold();
    TEST_ASSERT_TRUE(detected);
}
```

### Running UNITY Tests

```bash
# Build and run tests
gcc -o test_runner test_sensor.c unity.c -I.
./test_runner

# Output example:
# test_Sensor_ReadValidValue ... PASS
# test_Sensor_ReadTwice_SameValue ... PASS
# test_Sensor_ThresholdDetection ... PASS
# 
# Tests: 3, Passed: 3, Failed: 0
```

### Mocking External Dependencies

```c
// Mock ADC for testing without hardware
#include "unity.h"
#include "Mockado.h"
#include "sensor.h"

void setUp(void)
{
    Mockado_Init();
}

void test_Sensor_ReadsFromADC(void)
{
    // Set ADC mock to return 2500
    ADC_Read_ExpectAndReturn(CH0, 2500);
    
    uint16_t value = Sensor_GetValue();
    TEST_ASSERT_EQUAL(2500, value);
    
    // Verify mock was called
    ADC_Read_IgnoreAndReturn(0);
}
```

---

## Test Result Generator

### Result Logging

```c
// test_results.h
typedef struct {
    char test_name[64];
    uint32_t timestamp;
    bool passed;
    char error_msg[128];
} test_result_t;

typedef struct {
    test_result_t results[100];
    uint16_t count;
    uint16_t passed;
    uint16_t failed;
} test_report_t;

void TestResult_AddResult(const char *name, bool passed, const char *msg);
void TestResult_GenerateReport(const char *filename);

// Implementation
test_report_t g_report = {0};

void TestResult_AddResult(const char *name, bool passed, const char *msg)
{
    if (g_report.count >= 100) return;
    
    test_result_t *result = &g_report.results[g_report.count];
    strncpy(result->test_name, name, 63);
    result->timestamp = HAL_GetTick();
    result->passed = passed;
    if (msg) strncpy(result->error_msg, msg, 127);
    
    g_report.count++;
    if (passed) g_report.passed++;
    else g_report.failed++;
}

void TestResult_GenerateReport(const char *filename)
{
    FILE *f = fopen(filename, "w");
    
    fprintf(f, "Test Report\n");
    fprintf(f, "Total: %d, Passed: %d, Failed: %d\n\n",
           g_report.count, g_report.passed, g_report.failed);
    
    for (int i = 0; i < g_report.count; i++) {
        fprintf(f, "[%s] %s - %s\n",
               g_report.results[i].passed ? "PASS" : "FAIL",
               g_report.results[i].test_name,
               g_report.results[i].error_msg);
    }
    
    fclose(f);
}
```

---

## Interview Q&A

### Q1: Design ATE system for testing multiple device features.

**Answer**:
```c
typedef struct {
    const char *test_name;
    int (*test_func)(void);
} ate_test_t;

ate_test_t ate_tests[] = {
    {"GPIO_Test", ATE_GPIO_Test},
    {"ADC_Test", ATE_ADC_Test},
    {"UART_Test", ATE_UART_Test},
    {"I2C_Test", ATE_I2C_Test},
    {"Memory_Test", ATE_Memory_Test},
    {NULL, NULL}
};

int ATE_RunAllTests(void) {
    int passed = 0, failed = 0;
    
    for (int i = 0; ate_tests[i].test_func; i++) {
        int result = ate_tests[i].test_func();
        if (result == 0) {
            passed++;
            printf("PASS: %s\n", ate_tests[i].test_name);
        } else {
            failed++;
            printf("FAIL: %s\n", ate_tests[i].test_name);
        }
    }
    
    return (failed == 0) ? 0 : -1;
}
```

### Q2: How structure test cases for production ATE?

**Answer**:
- **Functional Tests**: Core functionality
- **Boundary Tests**: Edge conditions
- **Stress Tests**: Temperature, voltage extremes
- **Repeatability Tests**: Run 100+ times
- **Combination Tests**: Multiple features together

---

**End of ATE & Testing Guide**
