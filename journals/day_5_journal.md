# Day 5 Journal - July 11

## Goals for Today
1. Migrate the custom testing framework in Project 1 (`Collections_Library_Pawan`) to Google Test (`gtest`).
2. Ensure all data structures (`DynamicArray`, `LinkedList`, `HashMap`) pass the standard Google Test runner.
3. Commit the finalized Project 1 repository.

## What I Did
- I installed the `gtest` library into my MSYS2 environment using `pacman`.
- I went through `test_dynamic_array.cpp`, `test_linked_list.cpp`, and `test_hash_map.cpp` and manually replaced the custom `EXPECT_EQ` macros and global test trackers with the standard `#include <gtest/gtest.h>` setup.
- I modified the `CMakeLists.txt` files to link `GTest::gtest_main`, which allowed me to remove my custom `main()` test runner functions.
- I encountered a few syntax errors during the manual migration (missing includes and dangling else blocks from the old `main` function) which I carefully debugged and removed.
- I also caught a bug where I was manually tracking exceptions using a custom `tests_total++` variable inside a `catch` block. I removed this entirely since `gtest` automatically handles exception tracking.

## Outcomes
- All tests now successfully compile using `cmake --build .`
- Running `ctest -V` executes all test suites, and every single test across all three core data structures is passing beautifully!
- Project 1 is fully tested and ready to be committed.

