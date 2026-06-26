#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstdlib>   // Required for std::malloc and std::free
#include <stdexcept> // Required for std::out_of_range
#include <new>       // Required for placement new
#include <utility>   // Required for std::move

template <typename T>
class DynamicArray {
private:
    T* a_data;      // Pointer to the start of our raw heap memory
    int a_size;     // How many items are actually currently stored
    int a_capacity; // Total number of memory slots allocated before we need to resize

    // Private helper: doubles the capacity and moves elements to a new memory block
    void resize() {
        a_capacity *= 2;
        T* new_data = static_cast<T*>(std::malloc(a_capacity * sizeof(T)));
        
        // Move existing elements to the new block
        for (int i = 0; i < a_size; ++i) {
            // Placement new: construct a new T at the specific memory address new_data[i]
            // We use std::move to efficiently transfer ownership without copying (if T supports it)
            new(&new_data[i]) T(std::move(a_data[i]));
            
            // Explicitly call the destructor on the old object since it was moved from
            a_data[i].~T();
        }
        
        // Free the old raw memory block
        std::free(a_data);
        a_data = new_data;
    }

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
        // Destroy all alive objects
        for (int i = 0; i < a_size; ++i) {
            a_data[i].~T();
        }
        // Free the raw memory block allocated by malloc
        std::free(a_data);
    }

    // --- Rule of Five: Copy Semantics ---

    // Copy Constructor: Creates a deep copy of another array
    DynamicArray(const DynamicArray& other) {
        a_capacity = other.a_capacity;
        a_size = other.a_size;
        a_data = static_cast<T*>(std::malloc(a_capacity * sizeof(T)));
        
        // Deep copy the elements using placement new
        for (int i = 0; i < a_size; ++i) {
            new(&a_data[i]) T(other.a_data[i]);
        }
    }

    // Copy Assignment Operator: Assigns one array to another (e.g., arr1 = arr2;)
    DynamicArray& operator=(const DynamicArray& other) {
        // FIXED: Check for self-assignment to prevent destroying our own data!
        if (this == &other) {
            return *this;
        }

        // Destroy current objects and free current memory
        for (int i = 0; i < a_size; ++i) {
            a_data[i].~T();
        }
        std::free(a_data);
        
        // Copy the new state
        a_capacity = other.a_capacity;
        a_size = other.a_size;
        a_data = static_cast<T*>(std::malloc(a_capacity * sizeof(T)));
        
        // At this point, if it was self-assignment, other.a_data was just freed!
        // We will be copying garbage or causing a segfault.
        for (int i = 0; i < a_size; ++i) {
            new(&a_data[i]) T(other.a_data[i]);
        }
        
        return *this;
    }

    // --- Modifiers ---

    // Appends an item to the end of the array, resizing if necessary
    void append(const T& value) {
        if (a_size == a_capacity) {
            resize();
        }
        // Construct the object in the pre-allocated uninitialized memory slot
        new(&a_data[a_size]) T(value);
        a_size++;
    }

    // Inserts an item at a specific index, shifting subsequent elements to the right
    void insert(int index, const T& value) {
        if (index < 0 || index > a_size) {
            throw std::out_of_range("Insert index out of bounds");
        }
        if (a_size == a_capacity) {
            resize();
        }
        
        // Shift elements from right to left to make space without overwriting
        for (int i = a_size; i > index; --i) {
            new(&a_data[i]) T(std::move(a_data[i - 1]));
            a_data[i - 1].~T();
        }
        
        // Insert the new value at the now "empty" slot
        new(&a_data[index]) T(value);
        a_size++;
    }

    // Removes an item at a specific index, shifting subsequent elements to the left
    void remove(int index) {
        if (index < 0 || index >= a_size) {
            throw std::out_of_range("Remove index out of bounds");
        }
        
        // Shift elements from right to left to fill the gap
        for (int i = index; i < a_size - 1; ++i) {
            a_data[i].~T(); // Destroy the old object
            new(&a_data[i]) T(std::move(a_data[i + 1])); // Move next object into this slot
        }
        
        // Destroy the very last element since it was moved left
        a_data[a_size - 1].~T();
        
        a_size--; // FIXED: Correctly decrement the array size!
    }

    // Removes the last item in the array
    void popBack() {
        if (a_size == 0) {
            throw std::out_of_range("Cannot pop from empty array");
        }
        a_data[a_size - 1].~T();
        a_size--;
    }

    // --- Search & Utility ---

    // Returns the index of the first occurrence of the value, or -1 if not found
    int indexOf(const T& value) const {
        for (int i = 0; i < a_size; ++i) {
            if (a_data[i] == value) {
                return i;
            }
        }
        return -1;
    }

    // Returns true if the value exists in the array
    bool contains(const T& value) const {
        return indexOf(value) != -1;
    }

    // Clears all elements from the array but keeps the capacity intact
    void clear() {
        for (int i = 0; i < a_size; ++i) {
            a_data[i].~T();
        }
        a_size = 0;
        
        // FIXED: Do NOT free the memory!
        // We leave a_data alone so that the capacity remains intact and 
        // future append() calls don't crash.
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

    // Element Access: Bounds-checked access
    // Throws an exception if the index is out of bounds
    T& get(int index) {
        if (index < 0 || index >= a_size) {
            throw std::out_of_range("Index out of bounds");
        }
        return a_data[index];
    }

    // Element Access: Bounds-checked access (const version for read-only arrays)
    const T& get(int index) const {
        if (index < 0 || index >= a_size) {
            throw std::out_of_range("Index out of bounds");
        }
        return a_data[index];
    }

    // Element Access: Direct access for maximum speed (no bounds checking)
    T& operator[](int index) {
        return a_data[index];
    }

    const T& operator[](int index) const {
        return a_data[index];
    }
};

#endif // DYNAMIC_ARRAY_H
