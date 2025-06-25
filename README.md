# PP10

## Goal

In this exercise you will:

* Explore custom `struct` types and `typedef` in C.
* Link against existing system libraries (e.g., `-lm`).
* Create and evolve a custom C library from header-only to a precompiled static archive and install it system-wide.
* Install and use a third-party JSON library (`jansson`) via your package manager.
* Download, build, and install a GitHub-hosted library with a Makefile into standard include/lib paths.

**Important:** Start a stopwatch when you begin and work uninterruptedly for **90 minutes**. When time is up, stop immediately and record where you paused.

---

## Workflow

1. **Fork** this repository on GitHub.
2. **Clone** your fork locally.
3. Create a `solutions/` directory in the project root:

   ```bash
   mkdir solutions
   ```
4. For each task, add or modify source files under `solutions/`.
5. **Commit** and **push** your solutions.
6. **Submit** your GitHub repo link for review.

---

## Prerequisites

* GNU C compiler (`gcc`) and linker (`ld`).
* Make utility (`make`).
* `apt` or your distro’s package manager.

---

## Tasks

### Task 0: Exploring `typedef` and `struct`

**Objective:** Define and use a custom struct type with `typedef`.

1. Create `solutions/point.h` with:

   ```c
   typedef struct {
       double x;
       double y;
   } Point;
   ```
2. Create `solutions/point_main.c` that includes `point.h`, declares a `Point p = {3.0, 4.0}`, and prints its distance from origin using `sqrt(p.x*p.x + p.y*p.y)` (linking `-lm`).

#### Reflection Questions

1. **What does `typedef struct { ... } Point;` achieve compared to `struct Point { ... };`?**
With struct, a new variable is declared. This variable is then only valid once. With typef struck, the type of the variable is defined, and by calling the type, we can repeatedly declare new variables with the same structure.

2. **How does the compiler lay out a `Point` in memory?**

The variable Point consists of two double variables stored consecutively in memory. This is like any other two numbers of type Double.


### Task 1: Linking the Math Library (`-lm`)

**Objective:** Compile and link a program against the math library.

1. In `solutions/`, compile `point_main.c` with:

   ```bash
   gcc -o solutions/point_main solutions/point_main.c -lm
   ```
2. Run `./solutions/point_main` and verify it prints `5.0`.

#### Reflection Questions

1. **Why is the `-lm` flag necessary to resolve `sqrt`?**
Without the -lm i can´t linking to the math.h -header and sqrt is a part of math.h.

2. **What happens if you omit `-lm` when calling math functions?**

I got the error:
/usr/bin/ld: /tmp/cchiyKDX.o: in function `main':
point_main.c:(.text+0x51): undefined reference to `sqrt'
collect2: error: ld returned 1 exit status


### Task 2: Header-Only Library

**Objective:** Create a simple header-only utility library.

1. Create `solutions/libutil.h` with an inline function:

   ```c
   #ifndef LIBUTIL_H
   #define LIBUTIL_H
   static inline int clamp(int v, int lo, int hi) {
       if (v < lo) return lo;
       if (v > hi) return hi;
       return v;
   }
   #endif
   ```
2. Create `solutions/util_main.c` that includes `libutil.h`, calls `clamp(15, 0, 10)` and prints `10`.
3. Compile and run:

   ```bash
   gcc -o solutions/util_main solutions/util_main.c
   ./solutions/util_main
   ```

#### Reflection Questions

1. **What are the advantages and drawbacks of a header-only library?**
One of the advantages of a header-only library is that it can consist of small (or larger) functions. These no longer need to be written into each program; instead, the other programs access the header file. However, these functions should remain small, as otherwise the execution would always take a long time.
2. **How does `static inline` affect linkage and code size?**
This makes the program smaller in terms of memory requirements.
---

### Task 3: Precompiled Static Library

**Objective:** Convert the header-only utility into a compiled static library and link it manually.

1. Split `clamp` into `solutions/util.c` & `solutions/util.h` (remove `inline` and `static`).
2. Compile:

   ```bash
   gcc -c solutions/util.c -o solutions/util.o
   ```
3. Create the executable linking manually:

   ```bash
   gcc -o solutions/util_main_pc solutions/util.o solutions/util_main.c
   ```
4. Run `./solutions/util_main_pc` to verify output.

#### Reflection Questions

1. **Why must you include `solutions/util.o` when linking instead of just the header?**

The header only provides declarations, but util.o contains implemented symbol definitions. Without these, the compiler and linker cannot implement the code.
2. **What symbol resolution occurs at compile vs. link time?**

The compiler recognizes that main uses a function declared in the header. During linking, the compiler searches the object file for the corresponding functions. It then links these functions to the function part of main and merges them.

### Task 4: Packaging into `.a` and System Installation

**Objective:** Archive the static library and install it to system paths.

1. Create `libutil.a`:

   ```bash
   ar rcs libutil.a solutions/util.o
   ```
2. Move headers and archive:

   ```bash
   sudo cp solutions/util.h /usr/local/include/libutil.h
   sudo cp libutil.a /usr/local/lib/libutil.a
   sudo ldconfig
   ```
3. Compile a test program using system-installed lib:

   ```bash
   gcc -o solutions/util_sys solutions/util_main.c -lutil
   ```

   (assumes `#include <libutil.h>`)

#### Reflection Questions

1. **How does `ar` create an archive, and how does the linker find `-lutil`?**
The ar rcs creates static archive (.a), so the linker looks with -lutil → libutil.a in /usr/local/lib.
2. **What is the purpose of `ldconfig`?**
The ldconfig file is required so that the system can find the new library. This also allows dynamic libraries to be used. This tells the compiler where to look for these libraries.
---

### Task 5: Installing and Using `jansson`

**Objective:** Install a third-party JSON library and link against it.

1. Install via `apt`:

   ```bash
   sudo apt update && sudo apt install libjansson-dev
   ```
2. Create `solutions/json_main.c`:

   ```c
   #include <jansson.h>
   #include <stdio.h>
   int main(void) {
       json_t *root = json_pack("{s:i, s:s}", "id", 1, "name", "Alice");
       char *dump = json_dumps(root, 0);
       printf("%s\n", dump);
       free(dump);
       json_decref(root);
       return 0;
   }
   ```
3. Compile and run:

   ```bash
   gcc -o solutions/json_main solutions/json_main.c -ljansson
   ./solutions/json_main
   ```

#### Reflection Questions

1. **What files does `libjansson-dev` install, and where?**
this is installed:
The following NEW packages will be installed:
  libjansson-dev
0 upgraded, 1 newly installed, 0 to remove and 167 not upgraded.
Need to get 35.9 kB of archives.
After this operation, 145 kB of additional disk space will be used.
Get:1 http://archive.ubuntu.com/ubuntu noble/main amd64 libjansson-dev amd64 2.14-2build2 [35.9 kB]
Fetched 35.9 kB in 0s (273 kB/s)
Selecting previously unselected package libjansson-dev:amd64.
(Reading database ... 91459 files and directories currently installed.)
Preparing to unpack .../libjansson-dev_2.14-2build2_amd64.deb ...
Unpacking libjansson-dev:amd64 (2.14-2build2) ...
Setting up libjansson-dev:amd64 (2.14-2build2) ...
2. **How does the linker know where to find `-ljansson`?**
The linker looks for ljansson file libjansson.so or .a in the standard paths (/usr/lib, /usr/local/lib
---

### Task 6: Building and Installing a GitHub Library

**Objective:** Download, build, and install a library from GitHub using its Makefile.

1. Choose a small C library on GitHub (e.g., `sesh/strbuf`).
2. Clone and build:

   ```bash
   git clone https://github.com/sesh/strbuf.git
   cd strbuf
   make
   ```
3. Install to system paths:

   ```bash
   sudo make install PREFIX=/usr/local
   sudo ldconfig
   ```
4. Write `solutions/strbuf_main.c` that includes `strbuf.h`, uses its API, and prints a test string.
5. Compile and link:

   ```bash
   gcc -o solutions/strbuf_main solutions/strbuf_main.c -lstrbuf
   ./solutions/strbuf_main
   ```

#### Reflection Questions

1. **What does `make install` do, and how does `PREFIX` affect installation paths?**
Make install executes the Makefile
2. **How can you inspect a library’s exported symbols to verify installation?**
???
---

**Remember:** Stop after **90 minutes** and record where you stopped.
