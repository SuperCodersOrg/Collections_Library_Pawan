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

void test_rule_of_zero() {
    std::cout << "\n--- Testing Rule of Zero (Copy & Move) ---\n";
    HashMap<std::string, int> original;
    original.put("A", 1);
    original.put("B", 2);

    // Testing Copy Constructor
    // This relies entirely on our custom DynamicArray and LinkedList deep copying!
    HashMap<std::string, int> copied = original;
    EXPECT_EQ(2, copied.size(), "Copied map has correct size");
    EXPECT_EQ(1, copied.get("A"), "Copied map has correct data");
    
    // Modify copy, ensure original is unchanged
    copied.put("A", 99);
    EXPECT_EQ(1, original.get("A"), "Original map is unchanged (Deep Copy worked!)");

    // Testing Move Constructor
    HashMap<std::string, int> moved = std::move(original);
    EXPECT_EQ(2, moved.size(), "Moved map has correct size");
    EXPECT_EQ(2, moved.get("B"), "Moved map has correct data");
}

void test_bulk_operations() {
    std::cout << "\n--- Testing Bulk Operations (Stress Test) ---\n";
    HashMap<std::string, int> bulk;
    
    // Insert 50 elements
    for (int i = 0; i < 50; ++i) {
        bulk.put("Key" + std::to_string(i), i * 10);
        EXPECT_EQ(i + 1, bulk.size(), "Bulk put size check");
    }
    
    EXPECT_EQ(50, bulk.size(), "Total size is 50");
    EXPECT_EQ(490, bulk.get("Key49"), "Last element is correct");
    EXPECT_EQ(250, bulk.get("Key25"), "Middle element is correct");

    // Remove 25 elements
    for (int i = 0; i < 25; ++i) {
        bulk.remove("Key" + std::to_string(i));
        EXPECT_EQ(50 - i - 1, bulk.size(), "Bulk remove size check");
    }
    
    EXPECT_EQ(25, bulk.size(), "Size is 25 after bulk remove");
    
    bool threw = false;
    try {
        bulk.get("Key0");
    } catch (...) {
        threw = true;
    }
    EXPECT_TRUE(threw, "Key0 is successfully removed");
    EXPECT_EQ(250, bulk.get("Key25"), "Key25 is still present");
}

void test_contains_and_clear() {
    std::cout << "\n--- Testing Contains & Clear ---\n";
    HashMap<std::string, int> map;
    map.put("X", 100);
    map.put("Y", 200);

    EXPECT_TRUE(map.contains("X"), "Map contains X");
    EXPECT_TRUE(map.contains("Y"), "Map contains Y");
    EXPECT_TRUE(!map.contains("Z"), "Map does not contain Z");

    map.clear();
    EXPECT_EQ(0, map.size(), "Size is 0 after clear");
    EXPECT_TRUE(!map.contains("X"), "Map does not contain X after clear");
}

void test_operator_brackets() {
    std::cout << "\n--- Testing Operator[] ---\n";
    HashMap<std::string, int> map;
    
    // Test insertion via operator[]
    map["NewKey"] = 777;
    EXPECT_EQ(1, map.size(), "Size is 1 after operator[] insertion");
    EXPECT_EQ(777, map.get("NewKey"), "Value is correctly set via operator[]");
    
    // Test retrieval and update via operator[]
    EXPECT_EQ(777, map["NewKey"], "Operator[] retrieves correct value");
    map["NewKey"]++;
    EXPECT_EQ(778, map["NewKey"], "Operator[] allows in-place mutation");
}

// Bug #3: HashMap::hash() can produce negative indices due to size_t → int truncation
void test_negative_hash() {
    std::cout << "\n--- Testing Negative Hash Index (Bug #3) ---\n";
    HashMap<std::string, int> map;

    // These strings will produce large std::hash values that may truncate
    // to negative int values before the modulo operation
    bool no_crash = true;
    try {
        // Insert many varied keys to increase the chance of hitting a negative hash
        for (int i = 0; i < 100; ++i) {
            std::string key = "stress_key_" + std::to_string(i * 7919); // Use a prime multiplier
            map.put(key, i);
        }
        EXPECT_EQ(100, map.size(), "All 100 stress keys inserted without crash");
        
        // Verify we can retrieve them
        for (int i = 0; i < 100; ++i) {
            std::string key = "stress_key_" + std::to_string(i * 7919);
            EXPECT_EQ(i, map.get(key), "Stress key retrieval correct");
        }
    } catch (const std::out_of_range& e) {
        no_crash = false;
        std::cout << "[FAIL] HashMap crashed with out_of_range: " << e.what() << "\n";
        tests_total++;
    } catch (...) {
        no_crash = false;
        std::cout << "[FAIL] HashMap crashed with unknown exception\n";
        tests_total++;
    }
    EXPECT_TRUE(no_crash, "No crash from negative hash indices");
}

// Bug #5: HashMap should be usable through const references
void test_const_correctness() {
    std::cout << "\n--- Testing Const Correctness (Bug #5) ---\n";
    HashMap<std::string, int> map;
    map.put("X", 100);
    map.put("Y", 200);

    // Pass by const reference — read methods should compile and work
    const HashMap<std::string, int>& cref = map;
    EXPECT_EQ(2, cref.size(), "Const size works");
    EXPECT_TRUE(!cref.isEmpty(), "Const isEmpty works");
    EXPECT_EQ(16, cref.getCapacity(), "Const getCapacity works");

    // These must compile on a const reference
    EXPECT_EQ(100, cref.get("X"), "Const get works");
    EXPECT_TRUE(cref.contains("X"), "Const contains works");
    EXPECT_TRUE(!cref.contains("Z"), "Const contains returns false for missing");
}

int main() {
    std::cout << "Starting HashMap Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_put();
    test_get_and_remove();
    test_resize();
    test_rule_of_zero();
    test_bulk_operations();
    test_contains_and_clear();
    test_operator_brackets();
    test_negative_hash();
    test_const_correctness();
    
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
