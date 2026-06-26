# Day 1 Journal: Dynamic Array Implementation

## Date: June 26, 2026
**Collection:** `DynamicArray<T>`

## 1. Architectural Decisions
Today I built the `DynamicArray` from scratch. I decided to make it a template class so it can hold any data type. Since C++ templates require the implementation to be visible to the compiler at the point of instantiation, I adopted a **header-only architecture**. The entire class and its methods are implemented in `include/DynamicArray.h`. 

For memory management, I deliberately chose to use `std::malloc` and `std::free` combined with **Placement New** and explicit destructor calls (`~T()`). This was a critical architectural decision. If I had used `new T[capacity]`, the compiler would have forced the type `T` to have a default constructor, which limits the flexibility of the collection. By using raw memory and placement new, our `DynamicArray` can store complex objects that do not have default constructors.

## 2. Major Bugs & Pitfalls Encountered
Building a standard library container from the ground up exposed me to numerous classic C++ pitfalls. Here is a comprehensive reflection on the bugs I ran into today:

### Uninitialized Raw Memory (Constructor)
In my custom capacity constructor, I allocated memory using `malloc`, but forgot that C++ doesn't initialize raw memory to zero. I briefly assumed the memory was clean, which led to garbage values appearing in size calculations before I explicitly set `a_size = 0`.

### Return by Value vs. Return by Reference
When implementing `get(index)` and `operator[]`, I initially returned the element by value (`T get()`). When I tried to modify an element (e.g., `arr[0] = 50;`), it didn't work because it was modifying a temporary copy of the element, not the actual element in the array. I had to change the signature to return by reference (`T& get()`).

### Pointer Invalidation during Resize
When I implemented the `resize()` function, I successfully allocated a new, larger memory block and copied the elements over. However, I completely forgot to update the `a_data` pointer to point to the new block (`a_data = new_data;`). The next time I appended an item, the program crashed with a segmentation fault because it was writing to memory that had already been freed.

### Destructive Forward Shifting (Insert)
When implementing `insert(index, value)`, I needed to shift elements to the right to make space. I instinctively wrote a forward loop (`for (int i = index; i < a_size; ++i)`). This caused a cascading overwrite because moving `a_data[i]` to `a_data[i+1]` destroyed the next element before it had a chance to move. I fixed this by reversing the loop to shift from right to left.

### Forgetting to Decrement Size (Remove)
In `remove()`, I correctly shifted elements left to fill the gap of the removed element, and I explicitly destroyed the lingering last element. But I forgot to actually decrement the tracking variable `a_size--`! The array behaved as if the item was never deleted, causing test failures.

### Fatal Segfault on Clear
When writing `clear()`, I fundamentally confused "destroying items" with "destroying capacity". I wrote `std::free(a_data);` inside `clear()`. This deallocated the entire memory block. The next time the test tried to `append()` to the cleared array, the program crashed instantly because `a_data` was null/invalid. I fixed it by only destroying the elements and leaving `a_data` allocated for future use.

### Double-Free Corruption on Self-Assignment
While implementing the Rule of Five Copy Assignment Operator (`operator=`), I tested a self-assignment scenario (`arr = arr;`). My logic was set to clear its own memory first before copying the other array. Since it was assigning to itself, it destroyed its own data and then tried to copy from the garbage it just freed. I added `if (this == &other) return *this;` to fix this.

### Stolen Pointer Use-After-Free (Move Semantics)
During the move semantics implementation, I successfully "stole" the memory pointer from the temporary object (`a_data = other.a_data;`). However, I forgot to neutralize the temporary object by setting `other.a_data = nullptr;`. When the temporary object went out of scope, its destructor called `free()` on the memory I had just stolen, resulting in a fatal double-free crash.

### Uninitialized State in Range Constructor
Finally, when implementing the iterator range constructor (`DynamicArray(InputIt first, InputIt last)`), I immediately jumped into a loop calling `append()`. I forgot that `append()` relies on the internal state (`a_size`, `a_capacity`, `a_data`) being initialized! Calling `append()` on garbage state caused an immediate crash until I added the proper initialization step.

## 3. Conclusion
Implementing `DynamicArray` was a rigorous exercise in manual memory management. It reinforced the importance of the Rule of Five, deep copying, move semantics, and careful pointer arithmetic. Reaching 79 passing test cases gives me extreme confidence in the robustness of this implementation. Tomorrow, we move on to `LinkedList`!
