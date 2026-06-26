#include <iostream>
#include <string>
#include "../include/DynamicArray.h"

// --- Custom Testing Framework ---
// We use simple global counters to track test success without external libraries.
int testsPassed = 0;
int testsFailed = 0;
int testCaseCount = 0;

// Helper to check if two values are equal
template <typename T>
void EXPECT_EQ(const T& expected, const T& actual, const std::string& testName) {
    testCaseCount++;
    if (expected == actual) {
        std::cout << "[PASS] " << testName << "\n";
        testsPassed++;
    } else {
        std::cout << "[FAIL] " << testName << " (Expected: " << expected << ", Got: " << actual << ")\n";
        testsFailed++;
    }
}

// Helper to check boolean conditions
void EXPECT_TRUE(bool condition, const std::string& testName) {
    testCaseCount++;
    if (condition) {
        std::cout << "[PASS] " << testName << "\n";
        testsPassed++;
    } else {
        std::cout << "[FAIL] " << testName << " (Expected True, Got False)\n";
        testsFailed++;
    }
}

// --- Test Cases ---

void test_initial_state() {
    std::cout << "\n--- Testing Initial State ---\n";
    
    // Create an array of integers
    DynamicArray<int> arr;
    
    // Check 1: A new array should have exactly 0 elements
    EXPECT_EQ(0, arr.size(), "Initial size is 0");
    
    // Check 2: Our chosen default capacity is 4 slots
    EXPECT_EQ(4, arr.capacity(), "Initial capacity is 4");
    
    // Check 3: isEmpty() should return true
    EXPECT_TRUE(arr.isEmpty(), "Array is empty initially");
}

int main() {
    std::cout << "Starting DynamicArray Tests...\n";
    
    // Run the tests
    test_initial_state();
    
    // Print Summary
    std::cout << "\n==============================\n";
    std::cout << "Tests Passed: " << testsPassed << " / " << testCaseCount << "\n";
    
    if (testsFailed > 0) {
        std::cout << "SOME TESTS FAILED!\n";
        return 1;
    }
    std::cout << "ALL TESTS PASSED!\n";
    return 0;
}
