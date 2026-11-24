# Memory Maps: Stack, Heap, Lifetimes, and Aliasing

**Programs analyzed:** `stack_example.c`, `heap_example.c`, `aliasing_example.c`  
**Build & environment:** Compiled on x86_64 Linux with `gcc -Wall -Wextra -Werror -pedantic -std=gnu89 -g` (with `-Wno-error=use-after-free` when testing runtime behavior on `aliasing_example.c`), executed **without Valgrind**.  
**Note on addresses:** Addresses below come from real program runs. Because ASLR randomizes base addresses on each execution, only **relative address offsets** and the **chronological order of events** are invariant.

**Notation:** `[S]` = Stack, `[H]` = Heap, `LIVE` / `DEAD` = whether an object's storage lifetime is active or has ended.

---

## 0. Ground Rules: Stack vs. Heap & Pointer Lifetimes

| Property | Stack Memory `[S]` | Heap Memory `[H]` |
| :--- | :--- | :--- |
| **Created by** | Function/block entry (automatic storage duration) | `malloc` / `calloc` / `realloc` (allocated storage duration) |
| **Destroyed by** | Function/block return (automatic) | `free` (manual deallocation) |
| **Lifetime boundary** | Ends immediately when the enclosing function or block exits | Ends at the exact `free()` call on that block, **not** when a pointer variable dies |
| **Examples here** | `local_int`, `local_buf`, `p_local`, `marker`, `a`, `b`, `n`, `arr`, `alice`, `bob`, `p` | The `int[5]` array, the two `Person` structs, the two name strings (`"Alice"`, `"Bob"`) |

Two core rules prevent most pointer reasoning errors:

1. **A pointer variable and its pointee have independent lifetimes.** In `aliasing_example.c`, `b` (`[S]`) lives until `main` returns, whereas the `int[5]` block (`[H]`) dies as soon as `free(a)` executes.
2. **`free(ptr)` does not modify `ptr` or its aliases.** It only ends the heap block's lifetime and returns it to the allocator. Both `a` and `b` retain the same numeric heap address (`0x5557869f82b0`), turning both into **dangling pointers**.

---

## 1. `stack_example.c` — Recursion, Stack Frames, and Lifetimes

### Observed Output (Trimmed)

```text
walk_stack depth  &marker            dump_frame &local_int   local_buf
0                 0x7fff4d6b4e94     (enter/exit) 0x...4e44   0x...4e50
1                 0x7fff4d6b4e64     (enter/exit) 0x...4e14   0x...4e20
2                 0x7fff4d6b4e34     (enter/exit) 0x...4de4   0x...4df0
3                 0x7fff4d6b4e04     (enter/exit) 0x...4db4   0x...4dc0
```
*(In another run, `&marker` progressed `0x7ffed0c090d4` $\rightarrow$ `0x7ffed0c090a4` $\rightarrow$ `0x7ffed0c09074` $\rightarrow$ `0x7ffed0c09044`, showing the exact same `-0x30` stride.)*

### What the Numbers Prove

* **Downward stack growth:** Each recursive `walk_stack` level pushes a new activation record at a **lower** address, stepping down by `0x30` (48 bytes) per recursion level.
* **Independent frame locals:** Each recursive invocation of `walk_stack` owns its own `marker` (`0`, `10`, `20`, `30`) at four distinct stack addresses. There is no sharing across recursion depths.
* **Compiler-determined frame layout:** Inside `dump_frame`, `local_buf[16]` sits at a higher address (`...4e50`) than `local_int` (`...4e44`), showing that frame offsets are determined by compiler alignment/layout rules rather than source declaration order.
* **Intra-frame stack aliasing:** `p_local == &local_int` aliases a local stack variable within a single `dump_frame` invocation. This is safe only because `p_local` is never returned or stored past `dump_frame`'s return—both die together.
* **Frame address reuse across sequential calls:** `dump_frame("enter", d)` and `dump_frame("exit", d)` print **identical** addresses for `local_int`, `local_buf`, and `p_local`. They are two separate calls with separate object lifetimes, but because the `"enter"` frame is popped before `"exit"` is called at the same recursion depth, the `"exit"` frame occupies the same stack slot.

### Memory Map at Deepest Recursion (`dump_frame("enter", 3)`)

```text
high addresses
  [S] main frame
  [S] walk_stack(0)   marker=0   @ ...4e94   LIVE
  [S] walk_stack(1)   marker=10  @ ...4e64   LIVE
  [S] walk_stack(2)   marker=20  @ ...4e34   LIVE
  [S] walk_stack(3)   marker=30  @ ...4e04   LIVE
  [S] dump_frame(3)   local_int=103 @ ...4db4  LIVE
                      local_buf     @ ...4dc0  = "D"
                      p_local = ...4db4  (alias of local_int)
low addresses  (stack grows downward)
```

### Event-by-Event Lifetimes

| Execution Event | Objects Born (`LIVE`) | Objects Destroyed (`DEAD`) |
| :--- | :--- | :--- |
| `walk_stack(0, 3)` entered | `marker` (`0`) in frame 0 | — |
| `dump_frame("enter", 0)` called $\rightarrow$ returns | `local_int` (`100`), `local_buf` (`"A"`), `p_local` | All three die upon `dump_frame` return |
| `walk_stack(1..3)` entered | One `marker` per depth (`10`, `20`, `30`) | — |
| `dump_frame("exit", 3)` called $\rightarrow$ returns | Fresh `local_int` (`103`), `local_buf` (`"D"`), `p_local` at reused slot | All three die upon `dump_frame` return |
| `walk_stack(3)` $\rightarrow$ `walk_stack(0)` unwind | — | `marker(30)` dies first, then `20`, `10`, and `0` as each frame pops |

> **Key takeaway:** Popped stack memory is **not zeroed**; it is merely reclaimed for future frames. Any pointer escaping a function to a local variable would silently alias whatever future frame reuses that stack address.

---

## 2. `heap_example.c` — Two-Level Heap Ownership and Partial-Free Leak

### Observed Output

```text
alice=0x555f1a1992b0 name=0x555f1a1992d0 age=30
bob  =0x555f1a1992f0 name=0x555f1a199310 age=41
```

### Objects & Storage Locations

| Object | Region | Sample Address | Requested Size | Allocator / Owner |
| :--- | :--- | :--- | :--- | :--- |
| `alice` (pointer variable) | `[S]` `main` | stack | 8 bytes | Automatic (`main` frame) |
| `bob` (pointer variable) | `[S]` `main` | stack | 8 bytes | Automatic (`main` frame) |
| `Person` struct (Alice) | `[H]` | `...92b0` | 16 bytes (`char*` 8 + `int` 4 + 4 padding) | `malloc(sizeof(Person))` |
| `"Alice\0"` buffer | `[H]` | `...92d0` | 6 bytes (`5 + '\0'`) | `malloc(len + 1)` |
| `Person` struct (Bob) | `[H]` | `...92f0` | 16 bytes | `malloc(sizeof(Person))` |
| `"Bob\0"` buffer | `[H]` | `...9310` | 4 bytes (`3 + '\0'`) | `malloc(len + 1)` |

*(The `0x20`-byte spacing between consecutive heap blocks reflects glibc's minimum chunk size and metadata alignment, not the requested byte counts.)*

### Memory Map After Both `person_new` Calls

```text
  [S] main:  alice ──────────────► [H] Person@92b0 { name ─► [H] "Alice\0"@92d0 , age=30 }
             bob   ──────────────► [H] Person@92f0 { name ─► [H] "Bob\0"@9310   , age=41 }
```

### Deallocation Sequence & Lifetimes

| Statement | Memory Effect |
| :--- | :--- |
| `free(bob->name);` | Frees `"Bob\0"@9310` (`DEAD`). `bob->name` becomes a dangling pointer holding `...9310`, but is never read again. |
| `free(bob);` | Frees `Person@92f0` (`DEAD`). **Order is critical:** freeing `bob` before `bob->name` would destroy the only pointer to `"Bob\0"` and leak it. |
| `person_free_partial(alice);` | Calls `free(p)` only, freeing `Person@92b0` (`DEAD`). |
| *(after `person_free_partial`)* | `"Alice\0"@92d0` remains `LIVE` in the heap allocator, but **every pointer to it has been lost** because its sole reference (`alice->name`) lived inside the freed `Person@92b0` block. |
| `return 0;` | `alice` and `bob` (`[S]`) die as `main` exits. |

**Result:** **6 bytes in 1 block are definitely lost** (an orphaned heap allocation with zero live references).

---

## 3. `aliasing_example.c` — Heap Aliasing and Use-After-Free

### Compiler Diagnostic & Observed Runtime Output

With the strict project flags (`-Wall -Wextra -Werror -pedantic -std=gnu89 -g`), GCC's static analyzer blocks compilation with `-Werror=use-after-free` on lines 40, 42, 44, and 45 because `b` is used after `free(a)`. When compiled without `-Werror`, execution produces:

```text
a=0x5557869f82b0 b=0x5557869f82b0 a[2]=22 b[2]=22
after free(a): b=0x5557869f82b0 (dangling)
reading b[2]=-2042119340
wrote b[3]=1234
```

### Step-by-Step Memory Map

| Step | Stack (`[S]`) | Heap (`[H]`) |
| :--- | :--- | :--- |
| `a = make_numbers(5)` | `arr` dies when `make_numbers` returns, but its address value is copied into `main`'s `a = ...82b0` | `int[5]` @ `...82b0` = `{0, 11, 22, 33, 44}`, `LIVE` (20 bytes requested) |
| `b = a` | `a = ...82b0`, `b = ...82b0` | Unchanged. **No duplicate array is allocated:** `a` and `b` alias the same heap block. |
| `printf(a[2], b[2])` | Both `a` and `b` read index 2 (`offset +8`) | Reads `22` from `...82b0 + 8` |
| `free(a)` | `a` and `b` both still hold `...82b0` (both now **dangling**) | `int[5]` @ `...82b0` transitions to `DEAD` (returned to allocator) |
| `printf("%d", b[2])` | `b` dereferences `...82b0 + 8` | **Undefined behavior (use-after-free read):** returns `-2042119340` |
| `b[3] = 1234` | `b` dereferences `...82b0 + 12` | **Undefined behavior (use-after-free write):** corrupts freed chunk |
| `return 0` | `a` and `b` die on `main` exit | — |

### Why `b[2]` Printed `-2042119340` Instead of `22`

Once `free(a)` releases the chunk, glibc's allocator places it onto a free list (such as tcache/fastbin) and writes internal free-list pointers directly into the user payload area of the freed chunk (specifically the first 16 bytes on x86_64). Because `b[2]` and `b[3]` access bytes `8..15` of that freed block, reading `b[2]` reads allocator metadata rather than `22`, and writing `b[3] = 1234` silently corrupts allocator state—exiting with status `0` instead of crashing immediately.

---

## 4. Critical Review of AI Explanations & Manual Corrections

### Case 1: `stack_example.c` — Confusing Reused Frame Addresses with Live Object Persistence

* **Flawed AI explanation:** *"In `stack_example.c`, `p_local` at `[exit] depth=0` still points to the same `local_int` created at `[enter] depth=0`, and every `dump_frame` call gets its own unique stack address."*
* **Why it is wrong/incomplete:**
  1. `dump_frame("enter", 0)` and `dump_frame("exit", 0)` are two separate function calls. The `local_int` from `"enter"` ended its lifetime when `dump_frame("enter", 0)` returned; `"exit"` creates a brand-new `local_int` and sets its own `p_local` to `&local_int`.
  2. Addresses are unique per *active stack depth*, not per total call count—popped frames are reused by subsequent calls at the same depth, which is why `"enter"` and `"exit"` print identical addresses.
* **Correction:** Explicitly separate **storage lifetime** (per call) from **stack address** (per active stack position), and note that `p_local` only aliases the `local_int` belonging to the currently executing `dump_frame` call.

### Case 2: `heap_example.c` — Misidentifying Which Allocation Leaks

* **Flawed AI explanation:** *"`person_free_partial(alice)` leaks `alice` because it does not completely free the struct."*
* **Why it is wrong/incomplete:** `free(p)` inside `person_free_partial(alice)` **does** free the `Person` struct itself (`...92b0`). What actually leaks is the **separate 6-byte heap block** holding `"Alice\0"` (`...92d0`).
* **Correction:** Model `Person` as a two-level heap ownership structure. Freeing the outer `Person` struct first destroys the only pointer (`alice->name`) to the inner string buffer, leaving the 6-byte string buffer orphaned (**definitely lost**).

### Case 3: `aliasing_example.c` — Assuming `free()` Nullifies Pointers or Always Causes a Crash

* **Flawed AI explanation:** *"After `free(a)`, `b` becomes invalid/NULL, so reading `b[2]` causes a segmentation fault."*
* **Why it is wrong/incomplete:**
  1. `free(a)` never modifies pointer variables (`a` and `b` both keep `0x5557869f82b0`).
  2. Saying *"points to invalid memory"* obscures aliasing and lifetime: `b` is a dangling pointer to a freed heap chunk that is still mapped in the process's address space, which is why the program exits with code `0` while reading overwritten allocator metadata (`-2042119340`).
* **Correction:** Document the `a`/`b` alias explicitly, distinguish pointer value from heap object lifetime, and explain both GCC's `-Werror=use-after-free` static check and the runtime allocator metadata overwrite.

---

## 5. Summary Table

| Program | Stack Objects (`[S]`) | Heap Objects (`[H]`) | Aliasing | Defect | Runtime Behavior |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `stack_example.c` | 4 `marker` vars, 8 sequential `dump_frame` frames | None | `p_local == &local_int` within each `dump_frame` call | None | Stack grows downward by `0x30` per recursion level; popped frame addresses are reused |
| `heap_example.c` | `alice`, `bob`, `p` | 2 `Person` structs, 2 name strings | None | Memory leak | `"Alice\0"` (6 bytes) is orphaned when `Person` is freed first |
| `aliasing_example.c` | `a`, `b`, `n`, `arr` (in callee) | 1 `int[5]` array (20 bytes) | `a` and `b` hold the same heap address | Use-after-free (read & write via `b`) | Blocked by `-Werror=use-after-free`; without `-Werror`, reads allocator metadata (`-2042119340`) and exits `0` |