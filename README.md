# Repeated Substring Matcher (CSelfMatch)

This project implements a C++ template class, `CSelfMatch`, designed to efficiently find **repeated contiguous subsequences (substrings)** within a given sequence of elements. The core data structure used for efficient searching is a **Generalized Suffix Tree**.

The problem involves two main tasks:

1. **Finding the length of the longest substring** that occurs at least $N$ times.

2. **Finding all combinations of starting positions** for such a longest substring.

## Features

The `CSelfMatch` class supports sequences of any type (`T_`) that support copy, move, destruction, and equality comparison (`==` and `!=`).

### Public Interface

The `CSelfMatch` class offers the following public methods for analyzing the sequence:

* **`CSelfMatch(...)`**

  * **Constructors** that initialize the matcher from various inputs, including `std::initializer_list`, any standard C++ container, or an iterator pair.

* **`size_t sequenceLen(size_t n) const`**

  * Returns the length of the **longest contiguous subsequence** that occurs at least `n` times in the sequence.

  * *(Throws `std::invalid_argument` if* $n=0$*)*

* **`template<size_t N_> std::vector<std::array<size_t, N_>> findSequences() const`**

  * First, identifies the **longest subsequence** that satisfies the "occurs at least $N$ times" condition.

  * Returns a vector containing all unique combinations of $N$ starting positions where this longest subsequence occurs.

  * *(Throws `std::invalid_argument` if* $N=0$*)*

* **`void push_back(...)`** * Appends one or more elements to the sequence and rebuilds the Suffix Tree, allowing for dynamic extension of the analyzed sequence.

## Building and Running

This project can be compiled using a standard C++ compiler (like g++ or clang). A `Makefile` is provided for convenience.

### Prerequisites

* A C++ compiler supporting C++17 or later (for features like template argument deduction and `std::array`).

### Compilation

Navigate to the root directory of the project and use the provided `Makefile`:
```
make
```

This command compiles `solution.cpp` and `test.cpp` into an executable named `test`.

### Running Tests

Execute the compiled program to run the built-in assertions:
```
./test
```
If all tests pass, the program will exit successfully.

## Implementation Details

The core of the solution lies in the `SuffixTree` class, which implements a Suffix Tree data structure, specifically optimized for finding repeated patterns:

* **Suffix Tree Construction:** The implementation uses a simplified approach (e.g., an implicit Suffix Tree construction followed by end-of-sequence marking) to build the tree from the input sequence.

* **Finding Repeated Substrings:** The problem of finding the longest substring that appears at least $N$ times is reduced to finding the **deepest internal node** (or split point on a link) in the Suffix Tree whose subtree contains at least $N$ *leaves* (or end-of-suffix markers).

* **Counting Occurrences:** Each internal node in the tree stores the total count of suffixes (`substrings`) passing through it. This count is used to enforce the "at least $N$ times" requirement.

* **Position Retrieval:** Once the relevant node(s) is found, a traversal down to the leaves retrieves the actual starting indices (`beginIdx`) of all matching suffixes.

* **Combinations:** The resulting set of indices is then processed to generate all $\binom{M}{N}$ combinations of $N$ positions, where $M$ is the total number of occurrences of the longest substring.
