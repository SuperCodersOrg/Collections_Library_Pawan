#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstdlib> // Required for std::malloc and std::free

template <typename T>
class DynamicArray {
private:
    T* a_data;      // Pointer to the start of our raw heap memory
    int a_size;     // How many items are actually currently stored
    int a_capacity; // Total number of memory slots allocated before we need to resize

public:
    // Default constructor: creates an empty array with space for 4 items
    DynamicArray() {
        a_capacity = 4; // Start with a small capacity to save memory
        a_size = 0;     // No items are actually in the array yet
        
        // Allocate raw memory for 4 items on the heap.
        // We use malloc instead of 'new T[]' because we only want memory slots,
        // we do NOT want to construct default objects in those slots yet.
        a_data = static_cast<T*>(std::malloc(a_capacity * sizeof(T)));
    }

    // Custom constructor: creates an array with a specific initial capacity
    DynamicArray(int customCapacity) {
        a_capacity = customCapacity; // FIXED: Correctly initialize capacity
        a_size = 0;
        
        // Allocate raw memory for a_capacity items
        a_data = static_cast<T*>(std::malloc(a_capacity * sizeof(T)));
    }

    // Destructor: cleans up the heap memory when the array goes out of scope
    ~DynamicArray() {
        // Free the raw memory block allocated by malloc
        std::free(a_data);
    }

    // Returns the number of items currently in the array
    int size() const {
        return a_size;
    }

    // Returns the total capacity before a resize is needed
    int capacity() const {
        return a_capacity;
    }

    // Returns true if there are no items in the array
    bool isEmpty() const {
        return a_size == 0;
    }
};

#endif // DYNAMIC_ARRAY_H
