# Build Log

> For every work session add an entry to your build log.

## Template

**Date:** [Date]
**Duration:** [Minutes]

**Goal:**
[What were you trying to achieve?]

**Problem Encountered:**
[What went wrong? Include any bugs or obstacles]

**What I Tried:**
[Steps taken to debug or fix the issue]

**Outcome:**
[What was the final result or realization?]

---

## Log Entries

**Date:** June 26
**Duration:** 30 minutes

**Goal:**
Set up the initial project structure, CMake build system, and draft the Phase 0 design proposals.

**Problem Encountered:**
N/A (Scaffolding phase)

**What I Tried:**
N/A

**Outcome:**
Successfully established the Git repository, CMake configuration, and finalized the Phase 0 design documents for DynamicArray and LinkedList.

---

**Date:** June 26
**Duration:** 15 minutes

**Goal:**
Implement Step 2 (Raw Memory & Teardown): Overloaded constructor `DynamicArray(int customCapacity)` and destructor.

**Problem Encountered:**
In the constructor, I forgot to assign the `customCapacity` parameter to the member variable `a_capacity`. Because `int` is a primitive type, `a_capacity` contained a garbage memory value (4219224) which was passed to `malloc`. This caused the test case to fail.

**What I Tried:**
I analyzed the compiler warning (`unused parameter 'customCapacity'`) and the test output (`Got: 4219224`). I documented this memory initialization bug in `day_1_journal.md`.

**Outcome:**
I added `a_capacity = customCapacity;` to the constructor. The tests successfully compiled and passed, proving the memory allocation now matches the requested capacity.

---

**Date:** June 26
**Duration:** 10 minutes

**Goal:**
Implement Step 3 (Element Access): Methods `get(index)` and `operator[](index)` to read and write to the array.

**Problem Encountered:**
Compilation error: `lvalue required as left operand of assignment`. I was trying to test modifying an element (e.g., `arr[0] = 5;`), but the compiler rejected it.

**What I Tried:**
I realized I had written the method signature as `T operator[](int index)` instead of `T& operator[](int index)`. By returning by value instead of by reference, the array was handing back a copy of the item, making it impossible to modify the actual stored memory. 

**Outcome:**
I changed the return types to `T&` (reference), which immediately fixed the compiler error and allowed writing to the array. The bounds-checking exception for `get()` also passed perfectly.

---

**Date:** June 26
**Duration:** 25 minutes

**Goal:**
Implement Step 4 (Core Modifiers): Implement `append(value)` and the automatic `resize()` logic using placement new.

**Problem Encountered:**
Segmentation fault (core dumped) when running the massive append test (1000 items). The array would successfully append 4 items, but crashed exactly on the 5th item when the first resize triggered.

**What I Tried:**
I traced the crash into the `resize()` function. I found that I was freeing the old memory (`std::free(a_data);`) but I completely forgot to update the pointer! I was missing the line `a_data = new_data;`. As a result, the array kept trying to write to memory that had just been handed back to the OS.

**Outcome:**
Added `a_data = new_data;` at the end of the resize function. All 17 new append/resize tests passed! The array successfully scales to hold 1000 items without segfaulting. Total test cases are now up to 23.

---

**Date:** June 26
**Duration:** 40 minutes

**Goal:**
Implement Step 5 (Advanced Modifiers): Add `insert(index, value)` to place an element anywhere in the array.

**Problem Encountered:**
Logic Error / Data Corruption. When I tested inserting into the middle of the array, the elements after the insertion index were corrupted. They all became copies of the element at the insertion index!

**What I Tried:**
I wrote a forward loop `for (int i = index; i < a_size; ++i)` to shift elements right by 1 to make space. The bug was that moving `a_data[i]` to `a_data[i+1]` overwrote the very next element before it had a chance to move. It caused a cascading overwrite.

**Outcome:**
I reversed the loop to shift from right to left: `for (int i = a_size; i > index; --i)`. This cleanly shifted elements without destroying data. The 11 new insert tests passed flawlessly! We now have 34 test cases.

---

**Date:** June 26
**Duration:** 50 minutes

**Goal:**
Implement Step 6 (Modifiers): Implement `remove(index)` and `popBack()`.

**Problem Encountered:**
Logic Error / Unused Memory. After removing elements, I noticed that subsequent insertions or calls to `size()` were behaving as if the elements were never deleted! The tests explicitly failed because `size()` returned 5 when it should have been 4.

**What I Tried:**
I traced through the `remove()` logic. I had correctly shifted all the elements to the left and explicitly called the destructor on the last element (`a_data[a_size - 1].~T();`) to clean it up. However, I completely forgot to decrement `a_size`!

**Outcome:**
Added `a_size--;` at the end of both `remove()` and `popBack()`. This immediately fixed the logic, and all 11 new tests passed (including throwing an exception when popping an empty array). We are now at 45 test cases!

---

**Date:** June 26
**Duration:** 45 minutes

**Goal:**
Implement Step 7 (Search & Clear): Implement `contains()`, `indexOf()`, and `clear()`.

**Problem Encountered:**
Fatal Segmentation Fault. After calling `clear()`, the array would instantly crash the program the next time someone called `append()`. The test runner didn't even output a failure; the OS just killed the process.

**What I Tried:**
I realized I had written `std::free(a_data);` inside the `clear()` function. This is a severe misunderstanding of how capacity works. `clear()` is supposed to empty the *elements* (setting `a_size = 0`) but keep the *memory* (`a_capacity`) intact for future reuse. By freeing `a_data`, the next `append()` tried to write data into a deallocated, null pointer.

**Outcome:**
I removed `std::free(a_data)` and `a_data = nullptr` from `clear()`. It now simply loops to destroy the elements and sets `a_size = 0`. The tests now pass cleanly, and we officially reached **55 test cases**!

---

**Date:** June 26
**Duration:** 50 minutes

**Goal:**
Implement Step 8 (Rule of Five): Implement the Copy Constructor and Copy Assignment Operator for deep copying.

**Problem Encountered:**
Double Free Corruption / Garbage Data on Self-Assignment. When I wrote a test case that assigned an array to itself (`arr = arr;`), the test failed catastrophically. The array was reading completely random garbage data (e.g. `Got: 7610112`).

**What I Tried:**
I analyzed my `operator=` logic. My assignment operator was designed to first delete its own data, and then copy the new data. However, during self-assignment, the "new data" IS the "old data"! By destroying its own elements and freeing `a_data` first, it was literally erasing the very data it was trying to copy from.

**Outcome:**
I added a simple self-assignment check (`if (this == &other) return *this;`) at the very top of `operator=`. This immediately solved the problem. The deep copy tests now pass, bringing our test count to 66!

---

**Date:** June 26
**Duration:** 50 minutes

**Goal:**
Implement Step 9 (Rule of Five): Implement the Move Constructor and Move Assignment Operator (`&&`) to optimize performance by stealing pointers instead of deep copying.

**Problem Encountered:**
Double Free / Use After Free Error. The move semantics worked initially, but when the program shut down, it crashed with a fatal memory error.

**What I Tried:**
I traced the destructor calls. I realized that my move operations correctly "stole" the memory pointer (`a_data = other.a_data;`), but I completely forgot to set `other.a_data = nullptr;`. When the temporary `other` array went out of scope, its destructor called `std::free()` on its `a_data` pointer, which deleted the exact memory that I had just stolen!

**Outcome:**
I added `other.a_data = nullptr;`, `other.a_size = 0;`, and `other.a_capacity = 0;` to both move functions. This neutralizes the temporary object so its destructor does nothing. The tests now pass, and we have reached **74 test cases**.

---

**Date:** June 26
**Duration:** 50 minutes

**Goal:**
Implement Step 10 (Iterators & Range Constructor): Add `begin()`, `end()`, and a constructor to build the array from a range of iterators.

**Problem Encountered:**
Uninitialized Memory Read / Immediate Crash. When I tested the range constructor (`DynamicArray<int> fromRange(arr.begin(), arr.end());`), the test crashed immediately with a segmentation fault.

**What I Tried:**
I looked at my range constructor. I had simply written a loop: `for (auto it = first; it != last; ++it) { append(*it); }`. I forgot that `append()` relies on `a_size`, `a_capacity`, and `a_data` being initialized! Because they were completely uninitialized (garbage memory), `append()` tried to write data into a wild pointer.

**Outcome:**
I added the necessary initialization lines (`a_capacity = 4; a_size = 0; a_data = malloc(...)`) to the top of the range constructor before the loop. The range constructor and the C++ range-based for-loop (`for (int val : arr)`) now work perfectly. We are up to **79 test cases**!

---

**Date:** June 27
**Duration:** 30 minutes

**Goal:**
Implement Step 1-2 (LinkedList): Skeleton, `prepend()`, and `append()`.

**Problem Encountered:**
Null Pointer Dereference (Segmentation Fault). When appending to an empty list, the list correctly set `head = newNode;`, but the next time I called `append()`, it crashed on `tail->next = newNode;`.

**What I Tried:**
I traced the state of the list. On the first append, I didn't update `tail`. Since `tail` was `nullptr`, the second append tried to dereference a null pointer.

**Outcome:**
Added `tail = newNode;` inside the `if (isEmpty())` block of `append()`. The tests successfully pass now without crashing.

---

**Date:** June 27
**Duration:** 45 minutes

**Goal:**
Implement Step 5-6 (LinkedList): `insert()`, `removeFirst()`, `removeLast()`, and `remove()`.

**Problem Encountered:**
Infinite Loop / Hang / OS Crash. When testing the `remove(index)` method, the test suite completely froze and had to be killed by the OS.

**What I Tried:**
I analyzed the node disconnection logic in `remove(index)`. When removing a middle node, I wrote `prev->next = current;` and then `delete current;`. This meant I linked the previous node directly to the memory address I was about to free, creating a dangling pointer instead of bypassing it! When `clear()` ran in the destructor, it encountered the dangling pointer and tried to double-free it, corrupting the heap and crashing the program.

**Outcome:**
I changed it to `prev->next = current->next;` to properly bypass the node being removed before deleting it. All tests pass perfectly now. We have 9 test cases.

---

**Date:** June 27
**Duration:** 25 minutes

**Goal:**
Implement Step 9 (LinkedList): Search methods `contains()` and `indexOf()`.

**Problem Encountered:**
Logic Error / Premature Return. When testing `indexOf(200)` for an item that was in the middle of the list, the test failed and returned `-1` (not found).

**What I Tried:**
I checked the loop inside `indexOf`. I had added an `else { return -1; }` block inside the loop. This meant if the very first node didn't match the value, the function gave up immediately instead of checking the rest of the list!

**Outcome:**
I removed the `else` block so it correctly traverses the entire chain before giving up and returning `-1`. Tests pass perfectly! We now have 15 test cases.

---

**Date:** June 27
**Duration:** 40 minutes

**Goal:**
Implement Step 10a (LinkedList): Rule of Five Copy Constructor and Copy Assignment Operator for deep copying.

**Problem Encountered:**
Data Destruction on Self-Assignment. When testing `assigned = assigned;`, the list size dropped to 0 and all data was lost.

**What I Tried:**
I analyzed `operator=`. The first thing it does is `clear()` the existing list so it can make room for the new elements. Because `assigned` was copying from itself, `clear()` erased the very data it was supposed to copy!

**Outcome:**
I added a simple `if (this == &other) return *this;` at the beginning of the operator to prevent self-assignment destruction. The deep copy tests now pass, bringing us up to 21 test cases!

---

**Date:** June 27
**Duration:** 35 minutes

**Goal:**
Implement Step 10b (LinkedList): Rule of Five Move Constructor and Move Assignment Operator to steal pointers.

**Problem Encountered:**
Use-After-Free OS Crash. The move operations worked to steal the pointers (`head`, `tail`), but when the test completed and destructed the temporary source object, the program completely froze/crashed.

**What I Tried:**
I traced the destructor of the temporary object. It was calling `clear()`, which faithfully iterated through the list and `delete`d every node. However, since I stole those nodes, my new list was now pointing to deallocated memory! 

**Outcome:**
I added `other.head = nullptr`, `other.tail = nullptr`, and `other.size = 0` to both move functions. This neutralizes the source object so its destructor does nothing. Tests pass! 24 test cases.

---

**Date:** June 27
**Duration:** 30 minutes

**Goal:**
Implement Step 11 (LinkedList): Iterators, Range Constructor, and Stress Testing.

**Problem Encountered:**
No major bugs! The iterator implementation (`operator*`, `operator++`, `operator!=`) went smoothly, allowing C++ range-based for-loops (`for (int val : list)`) to work natively.

**What I Tried:**
I wrote a stress test inserting 50 items and verified that `size()`, `get()`, `contains()`, and `indexOf()` scale correctly without crashing.

**Outcome:**
The LinkedList is officially complete! We hit **82 test cases**, proving its robust memory management and logic. Moving on to Phase 2: HashMap.
