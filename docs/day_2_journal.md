# Day 2 Journal: The Beauty of Synergy

Today was incredibly productive, but it wasn't without its challenges. The goal was to build a Singly-Linked List and then use it alongside yesterday's Dynamic Array to create a fully functioning Hash Map using separate chaining.

## The LinkedList Struggles

I started with the `LinkedList`, which seemed simple enough until I hit a massive roadblock during testing. When I wrote the `remove(index)` method, I made a classic pointer manipulation mistake. Instead of linking the `prev` node to `current->next`, I accidentally linked `prev` directly to `current` and then deleted `current`! This left a dangling pointer in the middle of the list. When the list's destructor ran and tried to `clear()` the memory, it tried to delete that memory address again, causing a horrific double-free OS crash. Lesson learned: always draw out pointer bypasses on paper!

Another tricky issue was the Move Semantics. I stole the `head` and `tail` pointers perfectly, but I forgot to neutralize the source object. So when the temporary object was destroyed at the end of the statement, it called `clear()` and deleted all the nodes I had just stolen. Fixing this was as simple as setting `other.head = nullptr` and `other.tail = nullptr`.

## The HashMap Architecture

Building the HashMap was where things got really exciting. Because I already had a bulletproof `DynamicArray` and a fully tested `LinkedList`, implementing the hash map logic was mostly a matter of assembling the pieces.

However, I did introduce a few nasty bugs:
1. **The Duplicate Key Flaw:** In my first pass at `put(key, value)`, I forgot to check if the key already existed. This meant `map.put("Alice", 25)` followed by `map.put("Alice", 26)` would just append two entries into the bucket. By implementing C++ Iterators in my `LinkedList` earlier in the day, I was able to easily write a `for (auto& pair : bucket)` loop to find and update existing keys in place.
2. **Artificial Size Inflation:** When I wrote `resize()`, I cleverly re-used my public `put()` method to move items from the old buckets to the new ones. What I forgot was that `put()` increments `current_size`. So rehashing 12 items artificially added 12 to the size! Setting `current_size = 0` before the rehash loop instantly fixed this.

## The Rule of Zero

My biggest win today was realizing the power of composability. When it came time to write the Copy and Move constructors for the HashMap, I realized... I didn't have to!

Because the HashMap's only dynamic state is `DynamicArray<LinkedList<KeyValuePair>>`, and both `DynamicArray` and `LinkedList` manage their own memory deeply and safely, the C++ compiler-generated default copy and move constructors for the HashMap work perfectly out of the box. This is the fabled **Rule of Zero** in action, and it feels like a massive architectural victory.

With 82 tests passing for LinkedList and 111 tests passing for HashMap, I'm calling today a resounding success!
