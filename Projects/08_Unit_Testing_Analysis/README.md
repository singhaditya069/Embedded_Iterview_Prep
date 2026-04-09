# Unit Testing & Static Analysis - Complete Implementation Guide

## Project Overview

This project covers UNITY testing framework with SonarQube static analysis for code quality assurance and comprehensive test coverage.

---

## Unit Testing with UNITY

### Test Categories

| Category | Purpose | Example |
|----------|---------|---------|
| **Unit Tests** | Test individual functions | Test ADC_Read() |
| **Integration Tests** | Test module interactions | Test ADC + Filter |
| **System Tests** | End-to-end testing | Full sensor pipeline |
| **Regression Tests** | Prevent reintroduction of bugs | Test previous fixes |

### Test Structure

```c
#include "unity.h"
#include "module_under_test.h"

// Pre-test setup
void setUp(void)
{
    // Initialize test environment
    Module_Init();
    // Reset mocks
    MockDependency_Reset();
}

// Post-test cleanup
void tearDown(void)
{
    // Cleanup resources
    Module_DeInit();
    // Verify all mocks were called as expected
    MockDependency_VerifyAllCalls();
}

// Test implementation
void test_Module_FunctionA_WithValidInput_ReturnsExpected(void)
{
    // Arrange: Setup inputs
    uint16_t input = 100;
    uint16_t expected = 200;
    
    // Act: Call function
    uint16_t result = Module_FunctionA(input);
    
    // Assert: Verify result
    TEST_ASSERT_EQUAL(expected, result);
}

void test_Module_FunctionB_WithInvalidInput_ReturnsError(void)
{
    // Arrange
    uint16_t invalid_input = 0xFFFF;
    
    // Act
    module_status_t status = Module_FunctionB(invalid_input);
    
    // Assert
    TEST_ASSERT_EQUAL(MODULE_ERROR, status);
}

void test_Module_State_TransitionsCorrectly(void)
{
    // State machine testing
    TEST_ASSERT_EQUAL(MODULE_STATE_IDLE, Module_GetState());
    
    Module_Start();
    TEST_ASSERT_EQUAL(MODULE_STATE_RUNNING, Module_GetState());
    
    Module_Stop();
    TEST_ASSERT_EQUAL(MODULE_STATE_IDLE, Module_GetState());
}
```

### UNITY Assertion Reference

```c
// Equality assertions
TEST_ASSERT_EQUAL(expected, actual);
TEST_ASSERT_EQUAL_INT(expected, actual);
TEST_ASSERT_EQUAL_HEX(expected, actual);
TEST_ASSERT_EQUAL_STRING(expected, actual);

// Comparison assertions
TEST_ASSERT_GREATER_THAN(floor, value);
TEST_ASSERT_LESS_THAN(ceiling, value);
TEST_ASSERT_EQUAL_FLOAT(expected, actual);

// Boolean assertions
TEST_ASSERT_TRUE(value);
TEST_ASSERT_FALSE(value);

// Pointer assertions
TEST_ASSERT_NULL(pointer);
TEST_ASSERT_NOT_NULL(pointer);

// Array assertions
TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, length);
TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, length);

// String assertions
TEST_ASSERT_EQUAL_STRING(expected, actual);
TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, message);
```

### Test Execution

```bash
# Compile test runner
gcc -o test_runner test_*.c unity.c -I. -DDEBUG

# Run tests
./test_runner

# Output:
# test_Module_FunctionA_WithValidInput_ReturnsExpected ... PASS
# test_Module_FunctionB_WithInvalidInput_ReturnsError ... PASS
# test_Module_State_TransitionsCorrectly ... PASS
#
# Tests: 3, Passed: 3, Failed: 0, Skipped: 0
```

---

## SonarQube Static Analysis

### SonarQube Metrics

| Metric | Threshold | Purpose |
|--------|-----------|---------|
| **Code Coverage** | > 80% | Ensure adequate testing |
| **Code Duplication** | < 3% | Detect copy-paste code |
| **Complexity** | < 15 (cyclomatic) | Identify complex functions |
| **Issues** | Critical: 0 | Quality gates |
| **Security Hotspots** | 0 critical | Security assurance |

### SonarQube Configuration

```xml
<!-- sonar-project.properties -->
sonar.projectKey=embedded_firmware
sonar.projectName=Embedded Firmware
sonar.projectVersion=1.0.0

sonar.sources=src
sonar.tests=tests
sonar.inclusions=src/**/*.c,src/**/*.h
sonar.exclusions=src/vendor/**

sonar.c.coverage.reportPaths=coverage/coverage.xml
sonar.coverage.jacoco.xmlReportPaths=coverage/coverage.xml

sonar.host.url=http://localhost:9000
sonar.login=YOUR_TOKEN
```

### Code Coverage with GCOV

```bash
# Compile with coverage flags
gcc -fprofile-arcs -ftest-coverage -o test test.c module.c

# Run tests
./test

# Generate coverage report
gcov module.c

# Create HTML report
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html

# Upload to SonarQube
sonar-scanner
```

### Quality Gate Integration

```bash
#!/bin/bash
# ci_pipeline.sh

# Run SonarQube analysis
sonar-scanner \
  -Dsonar.projectKey=firmware \
  -Dsonar.sources=src \
  -Dsonar.host.url=$SONAR_URL \
  -Dsonar.login=$SONAR_TOKEN

# Wait for quality gate result
sleep 10

# Check result
QUALITY=$(curl -s "$SONAR_URL/api/qualitygates/project_status?projectKey=firmware" \
  -H "Authorization: Bearer $SONAR_TOKEN" | jq -r '.projectStatus.status')

if [ "$QUALITY" != "OK" ]; then
    echo "Quality gate FAILED"
    exit 1
fi

echo "Quality gate PASSED"
```

---

## Code Quality Improvements

### Common Issues & Fixes

**Issue 1: High Cyclomatic Complexity**
```c
// Before: Too many branches
int Sensor_Validate(uint16_t value) {
    if (value > 0 && value < 4096) {
        if (value != last_value) {
            if (change < threshold) {
                if (time_stable > STABLE_MS) {
                    return VALID;
                }
            }
        }
    }
    return INVALID;
}

// After: Extract to helper functions
bool Sensor_IsInRange(uint16_t value) {
    return value > 0 && value < 4096;
}

bool Sensor_HasChanged(uint16_t value) {
    return value != sensor_state.last_value;
}

bool Sensor_IsStable(void) {
    return sensor_state.stable_time > STABLE_MS;
}

int Sensor_Validate(uint16_t value) {
    if (Sensor_IsInRange(value) && Sensor_HasChanged(value) &&
        Sensor_IsStable()) {
        return VALID;
    }
    return INVALID;
}
```

**Issue 2: Code Duplication**
```c
// Before: Duplicated code
void ADC_Channel1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC1->CR2 = ADC_CR2_ADON;
    ADC1->SMPR2 = ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1;
    ADC1->SQR3 = 0;
}

void ADC_Channel2_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  // Duplicated!
    ADC1->CR2 = ADC_CR2_ADON;             // Duplicated!
    ADC1->SMPR2 = ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1;
    ADC1->SQR3 = 1;
}

// After: Extract common code
void ADC_CommonInit(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC1->CR2 = ADC_CR2_ADON;
    ADC1->SMPR2 = ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1;
}

void ADC_Channel_Init(uint8_t channel) {
    ADC_CommonInit();
    ADC1->SQR3 = channel;
}
```

---

## Interview Q&A

### Q1: How design comprehensive test suite for embedded system?

**Answer**:
```
Test Pyramid Strategy:

        △ E2E Tests (5%)
       /\      - Full system tests
      /  \     - Production-like scenarios
     /    \
    /______\
   /        \  Integration (15%)
  /          \  - Module interactions
 /____________\ 

/            \ Unit Tests (80%)
/              \ - Individual functions
/                \ - Edge cases
/__________________\
```

**Test Plan**:
1. Unit tests: 80% coverage for core functions
2. Integration tests: Verify module interactions
3. System tests: End-to-end with actual hardware
4. Regression tests: Prevent bug reoccurrence

### Q2: How handle flaky tests in CI/CD pipeline?

**Answer**:
```c
// Retry mechanism
#define MAX_RETRIES 3

int test_with_retry(int (*test_func)(void)) {
    for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
        setUp();
        int result = test_func();
        tearDown();
        
        if (result == 0) {
            return 0;  // Pass
        }
        
        if (attempt < MAX_RETRIES - 1) {
            sleep(100 * (attempt + 1));  // Backoff
        }
    }
    return -1;  // Failed all retries
}

// Identify flakiness
void test_Sensor_Reading_Stable(void) {
    uint32_t readings[10];
    for (int i = 0; i < 10; i++) {
        readings[i] = Sensor_Read();
    }
    
    // Calculate variance
    double mean = calculate_mean(readings, 10);
    double variance = calculate_variance(readings, 10, mean);
    
    // Flaky if variance too high
    TEST_ASSERT_LESS_THAN(VARIANCE_THRESHOLD, variance);
}
```

### Q3: What are quality gates and how establish them?

**Answer**:
Quality gates are criteria that must pass before releasing code:

```
✓ Code Coverage > 80%
✓ No Critical Issues
✓ Cyclomatic Complexity < 15
✓ No Security Hotspots
✓ Code Duplication < 3%
✓ Documentation Complete
✓ All Tests Passing
```

Implementation in SonarQube:
1. Define quality gate in SonarQube admin
2. Link to CI/CD pipeline
3. Block merge if gate fails
4. Require manual override with justification

---

**End of Unit Testing & Analysis Guide**
