*This activity has been created as part of the 42 curriculum by arngomes, vfleskes.*

# push_swap

## Description

`push_swap` is a sorting program with a twist: the numbers live in two stacks, `a` and `b`, and
they can only be moved with a small set of operations. The program reads a list of integers
(stack `a`, first argument on top), works out how to sort it, and prints the operations that
sort `a` in ascending order with `b` empty at the end.

The allowed operations are:

| Op | Effect |
|---|---|
| `sa` / `sb` / `ss` | swap the first two elements of `a` / `b` / both |
| `pa` / `pb` | move the top of `b` onto `a` / the top of `a` onto `b` |
| `ra` / `rb` / `rr` | rotate up (the first element becomes the last) `a` / `b` / both |
| `rra` / `rrb` / `rrr` | rotate down (the last element becomes the first) `a` / `b` / both |

The goal is to print as few operations as possible. The project is really about **algorithmic
complexity measured in operations**: the binary contains four strategies (simple O(n²),
medium O(n√n), complex O(n log n) and an adaptive one), and you can pick one at runtime.
The adaptive strategy chooses between the other three by measuring how disordered the input is.

## Instructions

### Build

```sh
make        # builds ./push_swap
make clean  # removes object files
make fclean # removes object files and the binary
make re     # fclean + all
```

The program is compiled with `cc -Wall -Wextra -Werror`. It needs nothing besides the C
standard library (`read`, `write`, `malloc`, `free`, `exit`).

### Usage

```sh
./push_swap [--bench] [--simple | --medium | --complex | --adaptive] <integers...>
```

- Integers can be given as separate arguments (`3 2 1`), inside one argument (`"3 2 1"`), or
  both. A leading `+` or `-` is accepted.
- `--simple`, `--medium` and `--complex` force one strategy. `--adaptive` is the default.
- `--bench` prints statistics on **stderr** after sorting. The operations still go to stdout.
- With no arguments, or with flags only and no numbers, the program prints nothing.
- If an argument is not an integer, is outside the `int` range, or is a duplicate, the program
  prints `Error\n` on stderr.

Flags go before the numbers, in any order (`--bench --complex` or `--complex --bench`). An
unknown flag (`--foo`), `--bench` given twice, or two strategies (`--simple --medium`) give
`Error`.

### Examples

```sh
$> ./push_swap 3 2 1
sa
rra

$> ARG="4 67 3 87 23"; ./push_swap $ARG | ./checker_linux $ARG
OK

$> ./push_swap 1 2 2
Error

$> ARG=$(shuf -i 0-9999 -n 500 | tr '\n' ' '); ./push_swap --bench $ARG 2> bench.txt | wc -l
$> cat bench.txt
[bench] disorder: 50.34%
[bench] strategy: Adaptive / O(n log n)
[bench] total_ops: 4464
[bench] sa: 136 sb: 0 ss: 0 pa: 969 pb: 969
[bench] ra: 887 rb: 556 rr: 0 rra: 554 rrb: 393 rrr: 0
```

## Algorithms

### Shared groundwork

- **Parsing** (`convert_input.c`, `input_utils.c`, `list_init.c`): each argument is split on
  whitespace. Each token goes through an overflow-safe `atoi`. The program rejects anything that
  is not a full integer token, anything outside `[INT_MIN, INT_MAX]`, and duplicates. The stack
  is a singly linked list of `t_node`.
- **Index normalisation** (`assign_index`): each value gets its rank `0 … n-1`. From then on the
  algorithms work on ranks, so they don't depend on the actual values (negative or very large
  numbers behave like any others).
- **Disorder** (`compute_disorder.c`): the fraction of pairs `(i, j)`, `i < j`, with
  `a[i] > a[j]`, in other words the number of inversions divided by n(n-1)/2. It is measured on
  the initial stack, before any move: 0 means sorted, 1 means reversed.
- **Operations** (`sort_operations*.c`): each operation changes the list, prints its name and
  adds one to its counter for `--bench`. An operation that has nothing to do (for example `sa`
  on one element) prints nothing.

All the costs below count **printed operations**, as the subject requires, not CPU steps.

### 1. Simple: selection by min extraction, O(n²) (`sort_simple.c`)

Repeat until `a` is empty: find the smallest remaining rank, bring it to the top of `a` the
short way (`ra` if it is in the upper half, `rra` otherwise), then `pb` it. At the end, `b`
holds everything in descending order from the top, so `n × pa` rebuilds `a` sorted.

- **Bound**: the k-th extraction costs at most ⌊(n-k)/2⌋ rotations plus 1 push. The total is
  at most n²/4 + 2n operations, so **O(n²)**.
- **Space**: O(1) beyond the n nodes, which are moved and never copied.
- **Why**: easy to explain and to prove, and cheap when the small values are already near the
  top.

### 2. Medium: chunk-based sorting, O(n√n) (`sort_medium.c`)

**Phase 1, push by sliding window.** We keep a window of ranks `[pushed, pushed + chunk]`,
with `chunk = 1.5 × ⌊√n⌋`. While `a` is not empty:
- if the top rank is `≤ pushed`, do `pb` and then `rb`, so the smallest values sink to the
  bottom of `b`;
- otherwise, if it is inside the window, do `pb`, so it stays near the top of `b`;
- otherwise, `ra`.

**Phase 2, push back the maximum.** Each time, find the largest rank left in `b`, bring it up
with `rb` or `rrb`, whichever direction is shorter, then `pa`.

- **Bound, phase 1**: each element is pushed once (n `pb`, at most n `rb`). The window stays
  full while elements remain, so one full turn of `a` pushes at least `chunk` elements. That
  means at most n/chunk ≈ √n turns, each costing at most n `ra`, so O(n√n).
- **Bound, phase 2**: phase 1 leaves `b` roughly sorted in "V" shaped blocks of about √n
  elements. The next maximum is therefore never more than about √n positions from an end of
  `b`. That gives n × O(√n) rotations plus n `pa`, so O(n√n).
- **Total: O(n√n)**. **Space: O(1)** beyond the nodes.
- **Why 1.5 × √n**: with √n chunks of √n elements, the two phases cost about the same. We
  measured the 1.5 factor on random inputs: it gives about 580 operations for 100 numbers and
  about 5,100 for 500.

### 3. Complex: 3-way quicksort on two stacks, O(n log n) (`sort_complex*.c`)

Two stacks have **four ends**: the top and bottom of `a` (`TOP_A`, `BOT_A`) and the top and
bottom of `b` (`TOP_B`, `BOT_B`). We work on **chunks**. A chunk is a block of `size`
consecutive ranks `[lo, lo + size)` sitting at one of these four ends. Since ranks are
consecutive, the pivots are exact: no median to search for.

**Splitting a chunk.** We take each of its `size` elements off its end and send it to one of
the three other ends:
- *large*: rank ≥ `lo + 3·size/5` (about 40%)
- *medium*: rank ≥ `lo + size/4` (about 35%)
- *small*: the rest (about 25%)

| Chunk is at | large → | medium → | small → |
|---|---|---|---|
| `TOP_A` | `BOT_A` (`ra`) | `TOP_B` (`pb`) | `BOT_B` (`pb rb`) |
| `BOT_A` | `TOP_A` (`rra`) | `TOP_B` (`rra pb`) | `BOT_B` (`rra pb rb`) |
| `TOP_B` | `TOP_A` (`pa`) | `BOT_A` (`pa ra`) | `BOT_B` (`rb`) |
| `BOT_B` | `TOP_A` (`rrb pa`) | `BOT_A` (`rrb pa ra`) | `TOP_B` (`rrb`) |

**Recursion.** We sort the *large* part first, then the *medium*, then the *small*. Each
recursive call leaves its part sorted at the top of `a`, so each new part lands right on top
of the larger values sorted before it. Each end of a stack behaves like a stack itself (last
in, first out), and the recursion is depth-first. A pending chunk is therefore always the
last block put on its end, and we can take it back intact.

**Base case (size ≤ 3).** Bring the elements to the top of `a`, then sort them with `sa`,
or with `ra sa rra` when other elements are below them. That is at most 5 operations.

**Normalisation.** A chunk at `BOT_A` that is in fact all of `a` is treated as `TOP_A` (same
for `b`). This avoids useless `rra`/`rrb`.

- **Bound**: during a split, each element costs at most 3 operations (see the table). The
  largest part is at most 2/5 of the chunk + 1, so the recursion depth is
  `log_{5/2}(n) + O(1)`. Each level handles each element at most once. That gives at most
  `3n·log_{5/2}(n) + O(n)` operations, so **O(n log n)**, whatever the input.
- **Space**: O(log n) for the recursion stack, O(1) beyond that.
- **Why uneven thirds (25 / 35 / 40)**: from `TOP_A`, a large element costs 1 operation and a
  small one costs 2. Making the small part smaller lowers the total. We tested several splits
  on random inputs: 1/4 and 3/5 gave the best worst case.
- **Results**: about 650 operations for 100 numbers (worst 672 out of 1,500 random inputs) and
  about 4,460 for 500 (worst 4,497). Before, radix sort made 1,084 and 6,784.

### Small inputs (n ≤ 5) (`sort_small.c`)

3 elements are sorted with a fixed decision table (at most 2 operations). For 4 or 5 elements,
we push the 1 or 2 smallest to `b` the short way, sort the 3 left in `a`, then push back.
The worst case is 10 operations for 5 elements (we checked all 120 permutations), so it is
**O(1)**.

### Low-disorder method: LIS + cheapest insertion, O(n²) (`sort_low*.c`)

The adaptive strategy uses this method when the disorder is below 0.2. It takes advantage of
the order that is already there instead of ignoring it:

1. **Keep what is already sorted.** Read `a` as a circle, starting from rank 0, and compute
   its longest increasing subsequence (LIS) with the classic O(n²) dynamic programming. We use
   a circle because a rotated sorted list is already sorted "up to a rotation", so it costs
   almost nothing.
2. **Push the rest.** Go through `a` once: `ra` the elements in the LIS, `pb` the others. We
   stop as soon as every non-LIS element has been pushed.
3. **Insert at the cheapest place.** `a` stays sorted up to a rotation. For each element `v`
   of `b`, its target in `a` is the smallest rank greater than `v`, or the minimum if there is
   none. We compute the cost of bringing `v` to the top of `b` and its target to the top of `a`
   in four ways: both up (shared `rr`), both down (shared `rrr`), and the two mixed ways. We
   pick the cheapest element in `b`, apply its moves, and `pa`.
4. **Finish.** Rotate rank 0 to the top the short way.

- **Bound**: at most n `pb`. Each insertion costs less than n rotations (the "both up" option
  is max(position in a, position in b) < n), plus 1 `pa`. The final rotation is at most n/2.
  That gives at most n² + O(n) operations, so **O(n²)**.
- **Space**: O(n) (three int arrays of size n for the LIS, plus a rank-to-position table and
  a "next rank present in `a`" table, rebuilt in O(n) before each insertion).
- **In practice**: the fewer elements are out of place, the shorter `b` is, and each insertion
  is local. A rotated sorted list costs 1 operation. 500 numbers with a disorder of 0.13 take
  about 2,000 operations, and 500 numbers with a disorder of 0.17 about 3,100. With plain
  selection sort, that last case cost about 17,000.
- If an allocation fails, the program falls back to the simple strategy, which needs no
  memory.

### 4. Adaptive: choice driven by disorder (`sort_adaptive.c`)

| Regime | Condition | Method | Operation bound |
|---|---|---|---|
| tiny | n ≤ 5 | small-input sort | O(1), at most 10 ops |
| low | disorder < 0.2 | LIS + cheapest insertion | O(n²) |
| medium | 0.2 ≤ disorder < 0.5 | chunk sort | O(n√n) |
| high | disorder ≥ 0.5 | 3-way quicksort | O(n log n) |

**Why these thresholds**

- **< 0.2**: fewer than 20% of the pairs are inverted, so most elements are already in a
  long increasing run. The LIS keeps them in place, and only the few elements that are out of
  place go through `b`, each inserted locally. The O(n²) bound is the worst case. In practice
  this regime does better than the chunk sort and the quicksort on the same inputs.
- **0.2 to 0.5**: the input still has ordered runs, but they are scattered. The chunk sort
  gets cheaper as the input gets more ordered: for 500 numbers, about 3,500 operations at a
  disorder of 0.17 and about 5,100 at 0.5. The quicksort costs about the same whatever the
  input looks like (about 4,450). At the bottom of this range, chunks do better.
- **≥ 0.5**: random or close to reversed, so there is no order left to use. We want an
  O(n log n) bound that does not depend on how the input is arranged, so we use the
  quicksort.
- **n ≤ 5**: whatever the disorder, the cost is a small constant, so we use a dedicated sort
  that does better than any of the general methods. At most 10 operations is O(1), which fits
  the bound of every regime. `--bench` therefore shows the class of the regime that matches
  the measured disorder (for example `Adaptive / O(n√n)` for `4 67 3 87 23`, disorder 40%).

**Cost of the decision itself**: measuring the disorder is O(n²) CPU comparisons, but it
prints no operations, so it adds nothing to the operation count. It takes O(1) extra space.
The overall operation bound of the adaptive strategy is the bound of the regime it picks.

### Performance (measured with random inputs)

| Size | Simple | Medium | Complex | Adaptive |
|---|---|---|---|---|
| 100 | ~1,500 | ~580 | ~650 (worst 672) | ~610 (worst 666) |
| 500 | ~32,000 | ~5,100 | ~4,460 (worst 4,497) | ~4,800 (worst ~5,230) |

A random input has a disorder of about 0.5, so the adaptive strategy picks medium or complex
depending on which side of the threshold the input falls. In every case we stay under the
"excellent" thresholds (700 and 5,500).

### Bench mode

`--bench` prints, on stderr: the disorder (as a percentage with two decimals), the strategy
used with its complexity class, the total number of operations, and the count for each of
the 11 operations. Disorder is computed before the sort starts.

The counters are kept in a `static` array local to a function in `bench.c` (`op_counts`).
This is not a global variable: only that function can see the array, and the other functions
reach it by calling `op_counts()`. That way each operation can count itself without changing
the prototypes of the 11 operations.

## Contributions

| Area | arngomes | vfleskes |
|---|---|---|
| Parsing and error handling (`convert_input.c`, `input_utils.c`, `list_init.c`) | review, fixes | **main author** |
| Stack operations (`sort_operations*.c`) | return values and bench counting | **main author** |
| Simple strategy (`sort_simple.c`) and small inputs (`sort_small.c`) | adjustments | **main author** |
| Complex strategy (`sort_complex.c`, `sort_complex_utils.c`, `sort_complex_base.c`) | 3-way quicksort version | first version (radix) |
| Medium strategy, chunks (`sort_medium.c`, `sort_medium_utils.c`) | **main author** | contributions |
| Disorder metric (`compute_disorder.c`) | **main author** | contributions |
| Adaptive strategy (`sort_adaptive.c`) | **main author** | contributions |
| Low-disorder method (`sort_low.c`, `sort_low_cost.c`, `sort_low_lis.c`) | **main author** | review |
| Bench mode (`bench.c`) | **main author** | |
| Makefile, header, Norm compliance | **main author** | contributions |

We both reviewed and tested every algorithm, and each of us can explain any part of the code.

## Resources

- D. E. Knuth, *The Art of Computer Programming, Vol. 3: Sorting and Searching*: inversions,
  selection sort, radix sort.
- Cormen, Leiserson, Rivest, Stein, *Introduction to Algorithms*: chapter 8 on linear-time
  sorting (radix sort) and asymptotic notation.
- [Big-O notation, Wikipedia](https://en.wikipedia.org/wiki/Big_O_notation)
- [Inversion (discrete mathematics), Wikipedia](https://en.wikipedia.org/wiki/Inversion_(discrete_mathematics)):
  the basis of the disorder metric.
- [Radix sort, Wikipedia](https://en.wikipedia.org/wiki/Radix_sort): our first complex
  strategy.
- R. Sedgewick, *Quicksort with 3-way partitioning*, and
  [Quicksort, Wikipedia](https://en.wikipedia.org/wiki/Quicksort): the basis of the
  complex strategy.
- The `push_swap` visualizers and testers shared in the 42 community, used to check results
  and count operations.

### Use of AI

We used an AI assistant (Claude) for:
- **Review and testing**: a full review of the project against the subject. It wrote a Python
  checker and test scripts (all permutations up to 5 elements, 100 and 500 element benchmarks
  on random, almost sorted and reversed inputs, error cases, valgrind runs). These found weak
  spots that we then looked at ourselves.
- **Documentation**: drafting this README. We checked and corrected the explanations and
  complexity arguments ourselves.

Apart from that method, we designed and wrote the algorithms and the C code ourselves.
