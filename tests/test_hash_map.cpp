#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/HashMap.h"

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
    HashMap<std::string, int> map;
    EXPECT_EQ(0, map.size(), "Initial size is 0");
    EXPECT_TRUE(map.isEmpty(), "Map is empty initially");
}

void test_put() {
    std::cout << "\n--- Testing Put ---\n";
    HashMap<std::string, int> map;
    
    map.put("Alice", 25);
    map.put("Bob", 30);
    EXPECT_EQ(2, map.size(), "Size is 2 after two unique puts");
    
    // Testing the bug!
    map.put("Alice", 26); // Updating Alice's age
    
    // Due to the bug, it just appends a new KeyValuePair, so size becomes 3!
    EXPECT_EQ(2, map.size(), "Size should still be 2 after updating an existing key");
}

void test_get_and_remove() {
    std::cout << "\n--- Testing Get & Remove ---\n";
    HashMap<std::string, int> map;
    
    map.put("Apple", 100);
    map.put("Banana", 200);
    
    EXPECT_EQ(100, map.get("Apple"), "Get returns correct value for Apple");
    EXPECT_EQ(200, map.get("Banana"), "Get returns correct value for Banana");
    
    map.get("Apple") = 150; // Modify via reference
    EXPECT_EQ(150, map.get("Apple"), "Get returns updated value");
    
    map.remove("Apple");
    EXPECT_EQ(1, map.size(), "Size is 1 after remove");
    
    bool threw = false;
    try {
        map.get("Apple");
    } catch (const std::out_of_range&) {
        threw = true;
    }
    EXPECT_TRUE(threw, "Get throws exception for removed key");
}

void test_resize() {
    std::cout << "\n--- Testing Resize & Rehashing ---\n";
    HashMap<std::string, int> map;
    
    EXPECT_EQ(16, map.getCapacity(), "Initial capacity is 16");
    
    // Insert 12 items to trigger load factor >= 0.75 (12 / 16 = 0.75)
    for (int i = 0; i < 12; ++i) {
        map.put("Key" + std::to_string(i), i);
    }
    
    EXPECT_EQ(12, map.size(), "Size is 12 before resize");
    EXPECT_EQ(16, map.getCapacity(), "Capacity is still 16");
    
    // Insert the 13th item, triggering a resize
    map.put("Key12", 12);
    
    EXPECT_EQ(32, map.getCapacity(), "Capacity doubled to 32");
    
    // BUG! Because we used put() which increments current_size, the size will be artificially inflated!
    EXPECT_EQ(13, map.size(), "Size should be exactly 13 after resize");
    
    EXPECT_EQ(12, map.get("Key12"), "Can still get elements after resize");
    EXPECT_EQ(0, map.get("Key0"), "Can still get elements after resize");
}

int main() {
    std::cout << "Starting HashMap Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_put();
    test_get_and_remove();
    test_resize();
    
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
