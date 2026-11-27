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
- With no arguments, the program prints nothing.
- If an argument is not an integer, is outside the `int` range, or is a duplicate, the program
  prints `Error\n` on stderr.

The flags have to come before the numbers, with `--bench` first.

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
[bench] disorder: 50.46%
[bench] strategy: Adaptive / O(n log n)
[bench] total_ops: 6784
[bench] sa: 0 sb: 0 ss: 0 pa: 2284 pb: 2284
[bench] ra: 2216 rb: 0 rr: 0 rra: 0 rrb: 0 rrr: 0
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

### 3. Complex: LSD binary radix sort, O(n log n) (`sort_complex.c`)

Ranks go from 0 to n-1, so they fit on `b = ⌈log₂ n⌉` bits. For each bit, from the least
significant one: go through all of `a` once, doing `pb` when the bit is 0 and `ra` when it
is 1, then `pa` everything back. Each pass is stable, so after the last bit `a` is sorted.

- **Bound**: each pass costs n operations (`pb` or `ra`) plus at most n `pa`. With
  `⌈log₂ n⌉` passes, that is at most 2n⌈log₂ n⌉ operations, so **O(n log n)**. For example
  6,784 operations for 500 numbers and 1,084 for 100.
- **Space**: O(1) beyond the nodes.
- **Why**: the bound holds whatever the input looks like, it is easy to prove, and the number
  of operations is almost the same on every input.

### Small inputs (n ≤ 5) (`sort_small.c`)

3 elements are sorted with a fixed decision table (at most 2 operations). For 4 or 5 elements,
we push the 1 or 2 smallest to `b` the short way, sort the 3 left in `a`, then push back.
The worst case is 10 operations for 5 elements (we checked all 120 permutations), so it is
**O(1)**.

### 4. Adaptive: choice driven by disorder (`sort_adaptive.c`)

| Regime | Condition | Method | Operation bound |
|---|---|---|---|
| tiny | n ≤ 5 | small-input sort | O(1), at most 10 ops |
| low | disorder < 0.2 | simple (selection) | O(n²) |
| medium | 0.2 ≤ disorder < 0.5 | chunk sort | O(n√n) |
| high | disorder ≥ 0.5 | radix sort | O(n log n) |

**Why these thresholds**

- **< 0.2**: fewer than 20% of the pairs are inverted, so most elements are already close to
  where they belong. The next minimum is usually close to the top of `a`, and each extraction
  costs far fewer than n/2 rotations. The O(n²) bound is the worst case. In practice this regime
  is cheap: about 200 operations for 100 numbers that are a rotation of a sorted list.
- **0.2 to 0.5**: the input still has long ordered runs, but they are scattered.
  Rank-windowed chunks pick up this partial order with few rotations, and they do better than
  radix, which does the same `2n log n` work whatever the input looks like.
- **≥ 0.5**: random or close to reversed. We want a guaranteed O(n log n) bound that does not
  depend on how the input is arranged, so we use radix.
- **n ≤ 5**: whatever the disorder, the cost is a small constant, so we use a dedicated sort
  that does better than any of the general methods.

**Cost of the decision itself**: measuring the disorder is O(n²) CPU comparisons, but it
prints no operations, so it adds nothing to the operation count. It takes O(1) extra space.
The overall operation bound of the adaptive strategy is the bound of the regime it picks.

### Performance (measured with random inputs)

| Size | Simple | Medium | Complex | Adaptive |
|---|---|---|---|---|
| 100 | ~1,500 | ~580 | 1,084 | 578 to 1,084 |
| 500 | ~32,000 | ~5,100 | 6,784 | 5,090 to 6,784 |

A random input has a disorder of about 0.5, so the adaptive strategy picks medium or complex
depending on which side of the threshold the input falls.

### Bench mode

`--bench` prints, on stderr: the disorder (as a percentage with two decimals), the strategy
used with its complexity class, the total number of operations, and the count for each of
the 11 operations. Disorder is computed before the sort starts.

## Contributions

| Area | arngomes | vfleskes |
|---|---|---|
| Parsing and error handling (`convert_input.c`, `input_utils.c`, `list_init.c`) | review, fixes | **main author** |
| Stack operations (`sort_operations*.c`) | return values and bench counting | **main author** |
| Simple strategy (`sort_simple.c`) and small inputs (`sort_small.c`) | adjustments | **main author** |
| Complex strategy, radix (`sort_complex.c`) | | **main author** |
| Medium strategy, chunks (`sort_medium.c`, `sort_medium_utils.c`) | **main author** | contributions |
| Disorder metric (`compute_disorder.c`) | **main author** | contributions |
| Adaptive strategy (`sort_adaptive.c`) | **main author** | contributions |
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
- [Radix sort, Wikipedia](https://en.wikipedia.org/wiki/Radix_sort)
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

The algorithms and the C code were designed and written by us. We used AI output only where we
understood it and could explain it.
