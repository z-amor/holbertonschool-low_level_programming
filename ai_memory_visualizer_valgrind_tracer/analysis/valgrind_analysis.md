# Valgrind & Memory Tracer Analysis

**Programs analyzed:** `aliasing_example.c` (`./a`), `heap_example.c` (`./h`)

**Commands executed:**
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./a
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./h
```
*(Compilation note: `aliasing_example.c` was compiled with `gcc -Wall -Wextra -pedantic -std=gnu89 -g aliasing_example.c -o a` without `-Werror` so GCC's static `-Wuse-after-free` warnings would not block building the binary for runtime tracing.)*

---

## 1. `aliasing_example.c` — Use-After-Free via Pointer Aliasing

### Valgrind Diagnostics Summary
* **Heap summary:** `in use at exit: 0 bytes in 0 blocks` (`2 allocs, 2 frees, 1,044 bytes allocated` — 20 bytes for the `int[5]` array plus 1,024 bytes for `stdio`'s internal stdout buffer).
* **Leak summary:** `All heap blocks were freed -- no leaks are possible`.
* **Error summary:** `3 errors from 3 contexts (suppressed: 0 from 0)` — three spatial/temporal invalid memory accesses on a freed heap block.

### Warning-by-Warning Classification & Memory Misuse Mapping

1. **Warning 1 — Invalid read of size 4 (`aliasing_example.c:42`)**
   * **Classification:** **Use-after-free (invalid read)** of 4 bytes (`sizeof(int)`).
   * **Memory object & address:** Address `0x4a9d488`, located **8 bytes inside** (`offset 2 * sizeof(int)`) a 20-byte heap block (`0x4a9d480`..`0x4a9d493`) originally allocated via `malloc` in `make_numbers` (`aliasing_example.c:12`, called from `main:30`) and deallocated via `free(a)` in `main` (`aliasing_example.c:38`).
   * **Causal memory misuse:** `a` received ownership of the 20-byte heap block on line 30, and `b = a` on line 34 created an alias pointing to the exact same base address (`0x4a9d480`). Calling `free(a)` on line 38 ended that heap allocation's lifetime without modifying `b`, leaving `b` as a **dangling pointer**. Evaluating `b[2]` (`*(b + 2)`) on line 42 dereferences that dangling pointer to read 4 bytes after the object's lifetime ended. *(Under Valgrind's Memcheck allocator, the freed payload still contained `22`, demonstrating how use-after-free bugs can appear to "succeed" without crashing.)*

2. **Warning 2 — Invalid write of size 4 (`aliasing_example.c:44`)**
   * **Classification:** **Use-after-free (invalid write)** of 4 bytes (`sizeof(int)`).
   * **Memory object & address:** Address `0x4a9d48c`, located **12 bytes inside** (`offset 3 * sizeof(int)`) the same freed 20-byte heap block (`0x4a9d480`).
   * **Causal memory misuse:** Executing `b[3] = 1234` writes a 4-byte integer into heap memory whose lifetime ended at line 38. In a standard allocator (such as glibc `ptmalloc`), writing into a freed chunk risks overwriting free-list metadata and corrupting the heap.

3. **Warning 3 — Invalid read of size 4 (`aliasing_example.c:45`)**
   * **Classification:** **Use-after-free (invalid read)** of 4 bytes (`sizeof(int)`).
   * **Memory object & address:** Address `0x4a9d48c`, located **12 bytes inside** the same freed 20-byte heap block (`0x4a9d480`).
   * **Causal memory misuse:** Evaluating `b[3]` as an argument to `printf` dereferences the dangling alias `b` a third time to read back the 4 bytes written on line 44 from deallocated heap storage.

---

## 2. `heap_example.c` — Memory Leak Due to Lost Ownership

### Valgrind Diagnostics Summary
* **Heap summary:** `in use at exit: 6 bytes in 1 blocks` (`5 allocs, 4 frees, 1,066 bytes allocated` — two 16-byte `Person` structs, a 6-byte `"Alice\0"` buffer, a 4-byte `"Bob\0"` buffer, and `stdio`'s 1,024-byte stdout buffer).
* **Leak summary:**
  * `definitely lost: 6 bytes in 1 blocks`
  * `indirectly lost: 0 bytes in 0 blocks`
  * `possibly lost: 0 bytes in 0 blocks`
  * `still reachable: 0 bytes in 0 blocks`
* **Error summary:** `1 errors from 1 contexts (suppressed: 0 from 0)` — zero invalid reads or writes, one definite memory leak.

### Warning Classification & Memory Misuse Mapping

* **Classification:** **Memory leak — Definitely lost** (`6 bytes in 1 blocks`, loss record 1 of 1).
* **Memory object & address:** The 6-byte heap character buffer (`5` characters for `"Alice"` + `1` byte for `'\0'`) at address `0x4a9d4d0`, allocated via `malloc(len + 1)` in `person_new` (`heap_example.c:21`) during `person_new("Alice", 30)` (`main:51`).
* **Causal memory misuse (leak due to lost ownership):**
  * Each `Person` instance owns a two-level heap hierarchy: the outer `Person` struct (`sizeof(Person)`) and an inner heap string buffer pointed to by `p->name`.
  * Bob's allocations are released in proper bottom-up ownership order: `free(bob->name)` (`main:64`) frees the inner string buffer (`0x4a9d570`) first while `bob` is still live, and `free(bob)` (`main:65`) frees the outer `Person` struct (`0x4a9d520`) second.
  * Alice is passed to `person_free_partial(alice)` (`main:67`), which executes `free(p)` (`heap_example.c:39`) on the outer `Person` struct (`0x4a9d480`) **without** freeing `p->name` (`0x4a9d4d0`) first.
  * Freeing `Person@0x4a9d480` ends the struct's lifetime and invalidates its internal `name` pointer field—the **only** pointer holding `0x4a9d4d0`. Because no live pointer anywhere in the program can reach `0x4a9d4d0` afterward, Valgrind classifies those 6 bytes as **definitely lost** due to lost ownership.

---

## 3. Critical Verification of AI Explanations

### Instance 1: Misidentifying Which Line Triggers Valgrind's First Error in `aliasing_example.c`

* **Flawed AI explanation:** When asked to predict and explain Valgrind's output for `aliasing_example.c`, an initial AI response stated: *"After `free(a)` resets `b`, Valgrind reports its first invalid read at line 40 (`printf("  after free(a): b=%p (dangling)\n", (void *)b);`) because `b` is used after `free(a)`."*
* **Why it is incorrect:**
  1. **`free()` does not reset pointers:** `free(a)` deallocates the heap block at `0x4a9d480`, leaving `b` completely unchanged as a dangling pointer holding `0x4a9d480`.
  2. **Reading a pointer's own value vs. dereferencing it:** Line 40 reads the value of the local stack variable `b` (`main`'s stack frame, which is still `LIVE`) and formats it with `%p`. It never dereferences `b` to access the freed heap block. While GCC's static analyzer emits a `-Wuse-after-free` warning on line 40 for using a pointer value after `free`, Valgrind's Memcheck instruments **memory accesses** and therefore reports zero errors on line 40—its first reported error occurs at **line 42** (`b[2]`), where `b` is actually dereferenced to read 4 bytes at `0x4a9d488`.

### Instance 2: Misidentifying the Leaked Object in `heap_example.c`

* **Flawed AI explanation:** Another AI summary stated: *"Valgrind reports 6 bytes definitely lost because `person_free_partial(alice)` fails to free `alice`."*
* **Why it is incorrect:** `person_free_partial(alice)` **does** call `free(p)` on `alice` (`0x4a9d480`), which is why `Person` (`sizeof(Person) == 16` bytes) is **not** in the leak summary. The leaked 6-byte block (`0x4a9d4d0`) is specifically the heap-allocated `"Alice\0"` string buffer owned by `alice->name`, orphaned because the parent struct was freed before its owned child allocation.

---

## 4. Summary Table

| Program | Valgrind Diagnostic | Classification | Involved Object & Offset | Root Memory Misuse |
| :--- | :--- | :--- | :--- | :--- |
| `aliasing_example.c` | `Invalid read of size 4` (`main:42`) | Use-after-free (read) | `int[5]` heap block (`0x4a9d480`), offset `+8` (`b[2]`) | Dereferencing dangling alias `b` after `free(a)` ended block lifetime |
| `aliasing_example.c` | `Invalid write of size 4` (`main:44`) | Use-after-free (write) | `int[5]` heap block (`0x4a9d480`), offset `+12` (`b[3]`) | Writing into freed heap chunk via dangling alias `b` |
| `aliasing_example.c` | `Invalid read of size 4` (`main:45`) | Use-after-free (read) | `int[5]` heap block (`0x4a9d480`), offset `+12` (`b[3]`) | Reading from freed heap chunk via dangling alias `b` |
| `heap_example.c` | `6 bytes in 1 blocks are definitely lost` (`person_new:21`) | Memory leak (lost ownership) | `"Alice\0"` heap buffer (`0x4a9d4d0`), 6 bytes | Freeing outer `Person` struct before freeing owned inner `p->name` pointer |