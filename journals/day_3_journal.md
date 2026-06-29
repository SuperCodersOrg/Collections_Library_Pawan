# Day 3 Journal: Audits, Optimizations, and WSL Debugging

Today was a massive day for hardening the core STL containers, resolving deep architectural bugs, and ultimately proving the memory safety of our implementations via Valgrind.

## 1. Addressing the Audit Bugs
We started the day by addressing five critical audit bugs discovered in our previous implementations. Before writing the fixes, we adopted a Test-Driven Development (TDD) approach by writing regression tests (`test_dynamic_array.cpp`, `test_hash_map.cpp`, `test_linked_list.cpp`) specifically designed to expose these flaws.

Once the tests were failing as expected, we went in and patched the core library:
* **Malloc Safety & Capacity Validation**: Fixed issues in `DynamicArray` where improper allocation or invalid capacities could lead to undefined behavior.
* **Hash Overflow**: Corrected overflow bugs in `HashMap` where negative hash values from signed truncation would cause out-of-bounds array accesses.
* **Const Correctness**: Ensured that our getters and iteration methods respected const correctness, especially across `LinkedList` and `HashMap`.

## 2. Performance Optimizations & Features
With the core bugs squashed, we focused on enhancements:
* **HashMap `operator[]` Optimization**: We noticed that the bracket operator was hashing the key multiple times. We refactored it to compute the hash exactly once, significantly speeding up lookups and insertions.
* **Feature Additions**: Implemented `getKeys` and `getValues` in `HashMap`, allowing users to easily extract iterables of the map's contents.
* **Move-Aware Append**: Updated `DynamicArray` to support a move-aware `append` operation, preventing unnecessary deep copies of objects when moving them into the array.

## 3. Structural Reorganization
We also did some housekeeping today, formally reorganizing our documentation by moving old journal entries out of the `docs` folder and into a dedicated `journals` directory.

## 4. The Valgrind Crusade
To guarantee that our `malloc` and `new` fixes actually eliminated memory leaks, we set out to run Valgrind. Since Valgrind requires Linux, we shifted our testing environment to the Windows Subsystem for Linux (WSL).

This transition wasn't without friction:
* We encountered a CMake `Operation not permitted` error due to running builds on the mounted Windows NTFS drive (`/mnt/c/`).
* We dealt with linker errors (`undefined reference to main`) because our `redis_lite` target pointed to an empty `main.cpp`. We sidestepped this by explicitly building the test targets instead (e.g., `make test_hashmap`).
* We discovered and patched a typo in `tests/CMakeLists.txt` where it was trying to build an empty `test_hashmap.cpp` instead of `test_hash_map.cpp`.

After navigating the WSL CMake quirks, we successfully ran CTest and Valgrind natively, confirming that our audit fixes were completely memory-safe. An incredibly productive day of bug squashing and hardening!
