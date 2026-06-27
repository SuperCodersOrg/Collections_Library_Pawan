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

void test_insert_and_remove() {
    std::cout << "\n--- Testing Insert & Remove ---\n";
    LinkedList<int> list;
    
    list.append(10);
    list.append(20);
    list.append(30);
    list.append(40);
    list.append(50);
    
    list.insert(2, 25);
    EXPECT_EQ(6, list.getSize(), "Size is 6 after insert");
    EXPECT_EQ(25, list.get(2), "Element 2 is 25");
    
    // Testing the buggy remove method!
    // We are trying to remove element at index 2 (value 25)
    list.remove(2);
    
    // Since we accidentally broke the link and deleted the node without bypassing it,
    // the list now has a dangling pointer.
    // The next node after 20 (index 1) will be reading from the freed memory of 25!
    EXPECT_EQ(5, list.getSize(), "Size is 5 after remove");
    EXPECT_EQ(30, list.get(2), "Element 2 should be 30 after removing 25");
}

void test_search() {
    std::cout << "\n--- Testing Search ---\n";
    LinkedList<int> list;
    list.append(100);
    list.append(200);
    list.append(300);

    // This will pass because 100 is at the head
    EXPECT_EQ(0, list.indexOf(100), "Index of 100 is 0");
    EXPECT_TRUE(list.contains(100), "List contains 100");

    // These will fail because of the premature return bug!
    EXPECT_EQ(1, list.indexOf(200), "Index of 200 is 1");
    EXPECT_TRUE(list.contains(200), "List contains 200");
    EXPECT_EQ(2, list.indexOf(300), "Index of 300 is 2");
    EXPECT_EQ(-1, list.indexOf(999), "Index of 999 is -1");
}

void test_copy_semantics() {
    std::cout << "\n--- Testing Copy Semantics ---\n";
    LinkedList<int> original;
    original.append(1);
    original.append(2);
    original.append(3);

    // Test Copy Constructor
    LinkedList<int> copied(original);
    EXPECT_EQ(3, copied.getSize(), "Copied list has correct size");
    EXPECT_EQ(2, copied.get(1), "Copied list has correct elements");
    
    // Modify copy, ensure original is unchanged
    copied.get(1) = 99;
    EXPECT_EQ(2, original.get(1), "Original list is unchanged (Deep Copy)");

    // Test Copy Assignment
    LinkedList<int> assigned;
    assigned.append(500); // Should be cleared
    assigned = original;
    EXPECT_EQ(3, assigned.getSize(), "Assigned list has correct size");
    EXPECT_EQ(1, assigned.get(0), "Assigned list has correct elements");

    // Test Self-Assignment Bug!
    assigned = assigned;
    EXPECT_EQ(3, assigned.getSize(), "Self-assigned list should keep its size");
}

void test_move_semantics() {
    std::cout << "\n--- Testing Move Semantics ---\n";
    LinkedList<int> original;
    original.append(100);
    original.append(200);

    // Test Move Constructor
    LinkedList<int> moved(std::move(original));
    EXPECT_EQ(2, moved.getSize(), "Moved list has correct size");
    EXPECT_EQ(200, moved.get(1), "Moved list has correct data");
    
    // The source (original) will now go out of scope and its destructor will fire!
    // BUG: Since we didn't neutralize 'original', it will delete the nodes
    // and 'moved' will now point to freed memory, causing a crash!

    // Test Move Assignment
    LinkedList<int> moved_assign;
    LinkedList<int> temp;
    temp.append(300);
    moved_assign = std::move(temp);
    EXPECT_EQ(1, moved_assign.getSize(), "Move assigned list has correct size");
}

int main() {
    std::cout << "Starting LinkedList Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_prepend_and_append();
    test_insert_and_remove();
    test_search();
    test_copy_semantics();
    test_move_semantics();
    
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
