# Day 4 Journal: The Redis CLI and C++ Deep Dives

Today's focus shifted from building core data structures to actually utilizing them in a real-world application layer: a Redis-like Command Line Interface.

## Building the CLI
We set out to implement standard Redis commands (`SET`, `GET`, `DEL`, `EXISTS`, `COUNT`, `CLEAR`, and `EXIT`) using our custom `HashMap` as the backend database. Initially, I structured `EXIT` as a dedicated method on the `RedisCLI` class. However, we quickly realized that `EXIT` is not a database operation—it's a control flow command for the session loop!

To fix this architectural flaw, we removed the `exit()` handler and implemented a `run()` method. This method contains an interactive Read-Evaluate-Print Loop (REPL) that reads user input from standard input, maps commands to `HashMap` methods, and gracefully breaks the loop when the user types `EXIT`.

## Compilation Woes and CMake Triumphs
While attempting to compile `RedisCLI.cpp` manually using `g++`, we ran into include path errors (`RedisCLI.h: No such file or directory`). While passing `-I../include` solves this for a quick check, manually compiling individual files isn't sustainable.

Instead, we completed the application architecture by creating a `src/main.cpp` entry point, uncommenting the `redis_lite` executable target in our `CMakeLists.txt`, and letting CMake handle the heavy lifting. The result? A fully functional, natively compiled `redis_lite.exe` running perfectly on Windows!

## A Deep Dive into C++ Template Optimizations
While reviewing some of the `DynamicArray` code, we had an interesting discussion about the `~T()` destructor calls and the `if constexpr (!std::is_trivially_destructible<T>::value)` pattern. 

Even though calling a pseudo-destructor on a primitive type like `int` is perfectly legal C++ (it just compiles to nothing), wrapping it in `if constexpr` is the modern, highly-optimized standard. It explicitly tells the compiler to completely strip out the destructor code during template instantiation for primitive types, yielding faster compilation times and crystal-clear developer intent.

The project is really coming together. We now have a high-performance, memory-safe STL backend powering a fast, interactive in-memory database CLI!
