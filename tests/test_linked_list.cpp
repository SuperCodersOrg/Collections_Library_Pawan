#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/LinkedList.h"

#include <gtest/gtest.h>
TEST(LinkedListTest, initial_state) {
    std::cout << "\n--- Testing Initial State ---\n";
    LinkedList<int> list;
    EXPECT_EQ(0, list.getSize()) << "Initial size is 0";
    EXPECT_TRUE(list.isEmpty()) << "List is empty initially";
}

TEST(LinkedListTest, prepend_and_append) {
    std::cout << "\n--- Testing Prepend & Append ---\n";
    LinkedList<int> list;
    
    list.prepend(10);
    EXPECT_EQ(1, list.getSize()) << "Size is 1 after prepend";
    
    // The second append will crash because tail is null!
    LinkedList<int> list2;
    list2.append(20);
    EXPECT_EQ(1, list2.getSize()) << "Size is 1 after first append";
    
    // Crash here:
    list2.append(30);
    EXPECT_EQ(2, list2.getSize()) << "Size is 2 after second append";
}

TEST(LinkedListTest, insert_and_remove) {
    std::cout << "\n--- Testing Insert & Remove ---\n";
    LinkedList<int> list;
    
    list.append(10);
    list.append(20);
    list.append(30);
    list.append(40);
    list.append(50);
    
    list.insert(2, 25);
    EXPECT_EQ(6, list.getSize()) << "Size is 6 after insert";
    EXPECT_EQ(25, list.get(2)) << "Element 2 is 25";
    
    // Testing the buggy remove method!
    // We are trying to remove element at index 2 (value 25)
    list.remove(2);
    
    // Since we accidentally broke the link and deleted the node without bypassing it,
    // the list now has a dangling pointer.
    // The next node after 20 (index 1) will be reading from the freed memory of 25!
    EXPECT_EQ(5, list.getSize()) << "Size is 5 after remove";
    EXPECT_EQ(30, list.get(2)) << "Element 2 should be 30 after removing 25";
}

TEST(LinkedListTest, search) {
    std::cout << "\n--- Testing Search ---\n";
    LinkedList<int> list;
    list.append(100);
    list.append(200);
    list.append(300);

    // This will pass because 100 is at the head
    EXPECT_EQ(0, list.indexOf(100)) << "Index of 100 is 0";
    EXPECT_TRUE(list.contains(100)) << "List contains 100";

    // These will fail because of the premature return bug!
    EXPECT_EQ(1, list.indexOf(200)) << "Index of 200 is 1";
    EXPECT_TRUE(list.contains(200)) << "List contains 200";
    EXPECT_EQ(2, list.indexOf(300)) << "Index of 300 is 2";
    EXPECT_EQ(-1, list.indexOf(999)) << "Index of 999 is -1";
}

TEST(LinkedListTest, copy_semantics) {
    std::cout << "\n--- Testing Copy Semantics ---\n";
    LinkedList<int> original;
    original.append(1);
    original.append(2);
    original.append(3);

    // Test Copy Constructor
    LinkedList<int> copied(original);
    EXPECT_EQ(3, copied.getSize()) << "Copied list has correct size";
    EXPECT_EQ(2, copied.get(1)) << "Copied list has correct elements";
    
    // Modify copy, ensure original is unchanged
    copied.get(1) = 99;
    EXPECT_EQ(2, original.get(1)) << "Original list is unchanged (Deep Copy)";

    // Test Copy Assignment
    LinkedList<int> assigned;
    assigned.append(500); // Should be cleared
    assigned = original;
    EXPECT_EQ(3, assigned.getSize()) << "Assigned list has correct size";
    EXPECT_EQ(1, assigned.get(0)) << "Assigned list has correct elements";

    // Test Self-Assignment Bug!
    assigned = assigned;
    EXPECT_EQ(3, assigned.getSize()) << "Self-assigned list should keep its size";
}

TEST(LinkedListTest, move_semantics) {
    std::cout << "\n--- Testing Move Semantics ---\n";
    LinkedList<int> original;
    original.append(100);
    original.append(200);

    // Test Move Constructor
    LinkedList<int> moved(std::move(original));
    EXPECT_EQ(2, moved.getSize()) << "Moved list has correct size";
    EXPECT_EQ(200, moved.get(1)) << "Moved list has correct data";
    
    // The source (original) will now go out of scope and its destructor will fire!
    // BUG: Since we didn't neutralize 'original', it will delete the nodes
    // and 'moved' will now point to freed memory, causing a crash!

    // Test Move Assignment
    LinkedList<int> moved_assign;
    LinkedList<int> temp;
    temp.append(300);
    moved_assign = std::move(temp);
    EXPECT_EQ(1, moved_assign.getSize()) << "Move assigned list has correct size";
}

TEST(LinkedListTest, iterators) {
    std::cout << "\n--- Testing Iterators & Range Constructor ---\n";
    LinkedList<int> list;
    list.append(5);
    list.append(10);
    list.append(15);

    int sum = 0;
    for (int val : list) {
        sum += val;
    }
    EXPECT_EQ(30, sum) << "Range-based for loop correctly iterates";

    LinkedList<int> fromRange(list.begin(), list.end());
    EXPECT_EQ(3, fromRange.getSize()) << "Range constructor created correct size list";
    EXPECT_EQ(10, fromRange.get(1)) << "Range constructor copied data correctly";
}

TEST(LinkedListTest, bulk_operations) {
    std::cout << "\n--- Testing Bulk Operations (Stress Test) ---\n";
    LinkedList<int> bulk;
    for (int i = 0; i < 50; ++i) {
        bulk.append(i * 10);
        EXPECT_EQ(i + 1, bulk.getSize()) << "Bulk append size check";
    }
    
    EXPECT_EQ(490, bulk.get(49)) << "Last element of bulk is correct";
    EXPECT_TRUE(bulk.contains(250)) << "Bulk contains 250";
    EXPECT_EQ(25, bulk.indexOf(250)) << "Index of 250 is correct";

    bulk.clear();
    EXPECT_EQ(0, bulk.getSize()) << "Size is 0 after bulk clear";
    EXPECT_TRUE(bulk.isEmpty()) << "List is empty after bulk clear";
}

// Bug #4: LinkedList should be usable through const references
TEST(LinkedListTest, const_correctness) {
    std::cout << "\n--- Testing Const Correctness (Bug #4) ---\n";
    LinkedList<int> list;
    list.append(10);
    list.append(20);
    list.append(30);

    // Pass by const reference — all read methods should compile and work
    const LinkedList<int>& cref = list;
    EXPECT_EQ(3, cref.getSize()) << "Const getSize works";
    EXPECT_TRUE(!cref.isEmpty()) << "Const isEmpty works";

    // These must compile on a const reference
    EXPECT_EQ(20, cref.get(1)) << "Const get works";
    EXPECT_TRUE(cref.contains(20)) << "Const contains works";
    EXPECT_EQ(2, cref.indexOf(30)) << "Const indexOf works";
    EXPECT_EQ(-1, cref.indexOf(999)) << "Const indexOf returns -1 for missing";

    // Const iteration via range-based for
    int sum = 0;
    for (const int& val : cref) {
        sum += val;
    }
    EXPECT_EQ(60, sum) << "Const range-based for loop works";
}

