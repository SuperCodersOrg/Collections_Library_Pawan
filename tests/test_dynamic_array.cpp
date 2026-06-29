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

void test_custom_capacity() {
    std::cout << "\n--- Testing Custom Capacity Constructor ---\n";
    
    // Create an array with a specific capacity of 100
    DynamicArray<int> arr(100);
    
    // Size should still be 0
    EXPECT_EQ(0, arr.size(), "Custom capacity size is 0");
    
    // Capacity should be 100
    EXPECT_EQ(100, arr.capacity(), "Custom capacity is 100");
}

void test_element_access() {
    std::cout << "\n--- Testing Element Access ---\n";
    DynamicArray<int> arr; // Empty array (size = 0)
    
    // Attempting to get an element out of bounds should throw std::out_of_range
    bool exceptionThrown = false;
    try {
        arr.get(0);
    } catch (const std::out_of_range& e) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown, "get() throws out_of_range on empty array");
}

void test_append_and_resize() {
    std::cout << "\n--- Testing Append & Resize ---\n";
    DynamicArray<int> arr; // Initial capacity is 4
    
    // Test appending a single item
    arr.append(10);
    EXPECT_EQ(1, arr.size(), "Size is 1 after first append");
    EXPECT_EQ(4, arr.capacity(), "Capacity is still 4");
    EXPECT_EQ(10, arr.get(0), "Element 0 is 10 (via get)");
    EXPECT_EQ(10, arr[0], "Element 0 is 10 (via operator[])");
    
    // Fill up to initial capacity (4)
    arr.append(20);
    arr.append(30);
    arr.append(40);
    EXPECT_EQ(4, arr.size(), "Size is 4 after 4 appends");
    EXPECT_EQ(4, arr.capacity(), "Capacity is still 4");
    EXPECT_EQ(40, arr.get(3), "Element 3 is 40");
    
    // Trigger the first resize (capacity should double to 8)
    arr.append(50);
    EXPECT_EQ(5, arr.size(), "Size is 5 after resize trigger");
    EXPECT_EQ(8, arr.capacity(), "Capacity doubled to 8");
    EXPECT_EQ(10, arr.get(0), "Element 0 is still 10 after resize");
    EXPECT_EQ(50, arr.get(4), "Element 4 is 50");
    
    // Trigger another resize by appending up to 9 items
    arr.append(60);
    arr.append(70);
    arr.append(80);
    arr.append(90); // 9th item -> resize to 16
    EXPECT_EQ(9, arr.size(), "Size is 9 after second resize trigger");
    EXPECT_EQ(16, arr.capacity(), "Capacity doubled to 16");
    EXPECT_EQ(90, arr.get(8), "Element 8 is 90");
    
    // Massive append to test large resizing
    DynamicArray<int> bigArr;
    for (int i = 0; i < 1000; ++i) {
        bigArr.append(i);
    }
    EXPECT_EQ(1000, bigArr.size(), "Large array size is 1000");
    EXPECT_EQ(1024, bigArr.capacity(), "Large array capacity is 1024 (2^10)");
    EXPECT_EQ(999, bigArr.get(999), "Last element is correct");
}

void test_insert() {
    std::cout << "\n--- Testing Insert ---\n";
    DynamicArray<int> arr;
    arr.append(10);
    arr.append(20);
    arr.append(30);
    
    // Insert at front
    arr.insert(0, 5); // Should become [5, 10, 20, 30]
    EXPECT_EQ(4, arr.size(), "Size is 4 after insert front");
    EXPECT_EQ(5, arr.get(0), "Element 0 is 5");
    EXPECT_EQ(10, arr.get(1), "Element 1 is 10");
    EXPECT_EQ(20, arr.get(2), "Element 2 is 20");
    EXPECT_EQ(30, arr.get(3), "Element 3 is 30");
    
    // Insert in middle
    arr.insert(2, 15); // Should become [5, 10, 15, 20, 30]
    EXPECT_EQ(5, arr.size(), "Size is 5 after insert middle");
    EXPECT_EQ(10, arr.get(1), "Element 1 is 10");
    EXPECT_EQ(15, arr.get(2), "Element 2 is 15");
    EXPECT_EQ(20, arr.get(3), "Element 3 is 20");
    
    // Insert at back (equivalent to append)
    arr.insert(5, 35); // Should become [5, 10, 15, 20, 30, 35]
    EXPECT_EQ(6, arr.size(), "Size is 6 after insert back");
    EXPECT_EQ(35, arr.get(5), "Element 5 is 35");
}

void test_remove_and_pop() {
    std::cout << "\n--- Testing Remove & PopBack ---\n";
    DynamicArray<int> arr;
    arr.append(10);
    arr.append(20);
    arr.append(30);
    arr.append(40);
    arr.append(50); // [10, 20, 30, 40, 50]
    
    // Remove from middle
    arr.remove(2); // Should remove 30 -> [10, 20, 40, 50]
    EXPECT_EQ(4, arr.size(), "Size is 4 after middle remove");
    EXPECT_EQ(10, arr.get(0), "Element 0 is 10");
    EXPECT_EQ(20, arr.get(1), "Element 1 is 20");
    EXPECT_EQ(40, arr.get(2), "Element 2 is 40");
    EXPECT_EQ(50, arr.get(3), "Element 3 is 50");
    
    // Remove from front
    arr.remove(0); // Should remove 10 -> [20, 40, 50]
    EXPECT_EQ(3, arr.size(), "Size is 3 after front remove");
    EXPECT_EQ(20, arr.get(0), "Element 0 is 20");
    
    // Test popBack
    arr.popBack(); // Should remove 50 -> [20, 40]
    EXPECT_EQ(2, arr.size(), "Size is 2 after popBack");
    EXPECT_EQ(40, arr.get(1), "Element 1 is 40");
    
    // Pop until empty
    arr.popBack();
    arr.popBack();
    EXPECT_EQ(0, arr.size(), "Size is 0 after popping all");
    
    bool exceptionThrown = false;
    try {
        arr.popBack();
    } catch (const std::out_of_range& e) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown, "popBack() throws out_of_range on empty array");
}

void test_search_and_clear() {
    std::cout << "\n--- Testing Search & Clear ---\n";
    DynamicArray<int> arr;
    arr.append(5);
    arr.append(10);
    arr.append(15);
    arr.append(20);
    
    // Test indexOf
    EXPECT_EQ(0, arr.indexOf(5), "indexOf(5) is 0");
    EXPECT_EQ(3, arr.indexOf(20), "indexOf(20) is 3");
    EXPECT_EQ(-1, arr.indexOf(99), "indexOf(99) is -1 (not found)");
    
    // Test contains
    EXPECT_TRUE(arr.contains(15), "Array contains 15");
    EXPECT_TRUE(!arr.contains(42), "Array does not contain 42");
    
    // Test clear
    int oldCapacity = arr.capacity();
    arr.clear();
    EXPECT_EQ(0, arr.size(), "Size is 0 after clear");
    EXPECT_EQ(oldCapacity, arr.capacity(), "Capacity remains unchanged after clear");
    EXPECT_TRUE(arr.isEmpty(), "Array is empty after clear");
    
    // Verify we can still append after clearing
    arr.append(100);
    EXPECT_EQ(1, arr.size(), "Size is 1 after appending to cleared array");
    EXPECT_EQ(100, arr.get(0), "Element 0 is 100");
}

void test_copy_semantics() {
    std::cout << "\n--- Testing Copy Semantics ---\n";
    DynamicArray<int> original;
    original.append(1);
    original.append(2);
    original.append(3);
    
    // Test Copy Constructor
    DynamicArray<int> copyConstructed = original;
    EXPECT_EQ(3, copyConstructed.size(), "Copy constructed size matches original");
    EXPECT_EQ(1, copyConstructed[0], "Element 0 copied correctly");
    EXPECT_EQ(3, copyConstructed[2], "Element 2 copied correctly");
    
    // Verify Deep Copy (modifying copy shouldn't affect original)
    copyConstructed[0] = 99;
    EXPECT_EQ(99, copyConstructed[0], "Copy element modified");
    EXPECT_EQ(1, original[0], "Original element remained unaffected (Deep Copy verified)");
    
    // Test Copy Assignment Operator
    DynamicArray<int> assigned;
    assigned.append(999); // give it some dummy data to ensure it gets cleared
    assigned = original;
    EXPECT_EQ(3, assigned.size(), "Assigned array size matches original");
    EXPECT_EQ(2, assigned[1], "Assigned element 1 is correct");
    
    // Verify Deep Copy for assignment
    assigned[2] = 500;
    EXPECT_EQ(500, assigned[2], "Assigned element modified");
    EXPECT_EQ(3, original[2], "Original element remained unaffected");
    
    // Self-Assignment Test (This will trigger the bug!)
    original = original;
    EXPECT_EQ(3, original.size(), "Self-assigned array size is intact");
    EXPECT_EQ(1, original[0], "Self-assigned array data is intact");
}

void test_move_semantics() {
    std::cout << "\n--- Testing Move Semantics ---\n";
    
    DynamicArray<int> source1;
    source1.append(100);
    source1.append(200);
    
    // Test Move Constructor
    DynamicArray<int> movedConstructed = std::move(source1);
    EXPECT_EQ(2, movedConstructed.size(), "Move constructed array has correct size");
    EXPECT_EQ(200, movedConstructed[1], "Move constructed array has correct data");
    
    // The source should now be completely empty and safe to destroy!
    EXPECT_EQ(0, source1.size(), "Source array size is 0 after move construction");
    EXPECT_EQ(0, source1.capacity(), "Source array capacity is 0 after move construction");
    
    DynamicArray<int> source2;
    source2.append(300);
    source2.append(400);
    
    // Test Move Assignment Operator
    DynamicArray<int> movedAssigned;
    movedAssigned.append(999); // Dummy data to be cleaned up
    movedAssigned = std::move(source2);
    
    EXPECT_EQ(2, movedAssigned.size(), "Move assigned array has correct size");
    EXPECT_EQ(400, movedAssigned[1], "Move assigned array has correct data");
    
    // The source2 should now be completely empty!
    EXPECT_EQ(0, source2.size(), "Source array size is 0 after move assignment");
    EXPECT_EQ(0, source2.capacity(), "Source array capacity is 0 after move assignment");
}

void test_iterators() {
    std::cout << "\n--- Testing Iterators & Range Constructor ---\n";
    DynamicArray<int> arr;
    arr.append(10);
    arr.append(20);
    arr.append(30);
    
    // Test Range-based for loop (relies on begin() and end())
    int sum = 0;
    for (int val : arr) {
        sum += val;
    }
    EXPECT_EQ(60, sum, "Range-based for loop successfully iterated and summed elements");
    
    // Test manual iterator usage
    DynamicArray<int>::Iterator it = arr.begin();
    EXPECT_EQ(10, *it, "begin() points to first element");
    
    // Test Range Constructor
    // This will trigger the crash because the range constructor is buggy!
    DynamicArray<int> fromRange(arr.begin(), arr.end());
    EXPECT_EQ(3, fromRange.size(), "Range constructed array has size 3");
    EXPECT_EQ(10, fromRange[0], "Range constructed element 0 is 10");
    EXPECT_EQ(30, fromRange[2], "Range constructed element 2 is 30");
}

// Bug #1: Validate that DynamicArray constructor rejects invalid capacity
void test_invalid_capacity() {
    std::cout << "\n--- Testing Invalid Capacity (Bug #2) ---\n";

    // Test zero capacity — should throw instead of creating a broken array
    bool threw_zero = false;
    try {
        DynamicArray<int> zero_cap(0);
    } catch (const std::invalid_argument&) {
        threw_zero = true;
    }
    EXPECT_TRUE(threw_zero, "DynamicArray(0) throws invalid_argument");

    // Test negative capacity — should throw instead of causing malloc to explode
    bool threw_neg = false;
    try {
        DynamicArray<int> neg_cap(-5);
    } catch (const std::invalid_argument&) {
        threw_neg = true;
    }
    EXPECT_TRUE(threw_neg, "DynamicArray(-5) throws invalid_argument");

    // Positive capacity should still work fine
    bool no_throw = true;
    try {
        DynamicArray<int> good_cap(8);
        EXPECT_EQ(8, good_cap.capacity(), "DynamicArray(8) has correct capacity");
    } catch (...) {
        no_throw = false;
    }
    EXPECT_TRUE(no_throw, "DynamicArray(8) does not throw");
}

int main() {
    std::cout << "Starting DynamicArray Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_custom_capacity();
    test_element_access();
    test_append_and_resize();
    test_insert();
    test_remove_and_pop();
    test_search_and_clear();
    test_copy_semantics();
    test_move_semantics();
    test_iterators();
    test_invalid_capacity();
    
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
