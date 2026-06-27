#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/LinkedList.h"

int tests_passed = 0;
int tests_total = 0;

void EXPECT_EQ(int expected, int actual, const std::string& test_name) {
    tests_total++;
    if (expected == actual) {
        std::cout << "[PASS] " << test_name << "\n";
        tests_passed++;
    } else {
        std::cout << "[FAIL] " << test_name << " (Expected: " << expected << ", Got: " << actual << ")\n";
    }
}

void EXPECT_TRUE(bool condition, const std::string& test_name) {
    tests_total++;
    if (condition) {
        std::cout << "[PASS] " << test_name << "\n";
        tests_passed++;
    } else {
        std::cout << "[FAIL] " << test_name << " (Expected True, Got False)\n";
    }
}

void test_initial_state() {
    std::cout << "\n--- Testing Initial State ---\n";
    LinkedList<int> list;
    EXPECT_EQ(0, list.getSize(), "Initial size is 0");
    EXPECT_TRUE(list.isEmpty(), "List is empty initially");
}

void test_prepend_and_append() {
    std::cout << "\n--- Testing Prepend & Append ---\n";
    LinkedList<int> list;
    
    list.prepend(10);
    EXPECT_EQ(1, list.getSize(), "Size is 1 after prepend");
    
    // The second append will crash because tail is null!
    LinkedList<int> list2;
    list2.append(20);
    EXPECT_EQ(1, list2.getSize(), "Size is 1 after first append");
    
    // Crash here:
    list2.append(30);
    EXPECT_EQ(2, list2.getSize(), "Size is 2 after second append");
}

int main() {
    std::cout << "Starting LinkedList Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_prepend_and_append();
    
    // Print Summary
    std::cout << "\n==============================\n";
    std::cout << "Tests Passed: " << tests_passed << " / " << tests_total << "\n";
    if (tests_passed == tests_total) {
        std::cout << "ALL TESTS PASSED!\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED!\n";
        return 1;
    }
}
