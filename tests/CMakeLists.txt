# Add test executables
add_executable(test_dynamic_array test_dynamic_array.cpp)
add_executable(test_linked_list test_linked_list.cpp)
add_executable(test_hashmap test_hash_map.cpp)

# Register tests with CTest
add_test(NAME DynamicArrayTest COMMAND test_dynamic_array)
add_test(NAME LinkedListTest COMMAND test_linked_list)
add_test(NAME HashMapTest COMMAND test_hashmap)

# Link Google Test
target_link_libraries(test_dynamic_array GTest::gtest_main)
target_link_libraries(test_linked_list GTest::gtest_main)
target_link_libraries(test_hashmap GTest::gtest_main)
