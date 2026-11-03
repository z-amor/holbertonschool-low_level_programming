Green Efficiency Analysis Report

Measurement Methodology

To systematically evaluate code execution efficiency, benchmarking experiments were conducted across three distinct C program tasks using a controlled Linux environment. All source code was compiled using standard strict compilation flags (-Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-long-long) without optimization flags to evaluate pure algorithmic complexity and execution overhead.

Execution times were recorded using the standard C library  facility via the clock() function. Runtimes were converted to seconds using the standard system macro CLOCKS_PER_SEC. To mitigate background system noise and CPU scheduling variations, each benchmark was executed across multiple consecutive iterations to determine timing stability and average execution performance.

Measurement boundaries were established strictly around computational loops and algorithm phases. In the multi-phase instrumentation experiment, timers were placed before and after each phase (BUILD_DATA, PROCESS, REDUCE) as well as across the overall program lifespan (TOTAL) to verify internal consistency and quantify system call overhead.

Observed Performance Differences

Across the recorded benchmarks, performance disparities highlighted the substantial impact of algorithmic complexity and operational weight:

1. Baseline Loop Stability : 
Across three consecutive runs of a deterministic $10^8$ iteration loop (baseline_loop.c), execution times yielded 0.176477s, 0.185758s, and 0.176340s. This demonstrated minor system scheduling variance (~9.4 ms peak variation) with an average execution time of ~0.179525 seconds.

2. Algorithmic Scaling Disparity :
The comparison benchmark (comparison_algorithms.c) evaluated a quadratic nested loop algorithm against a linear single-pass algorithm over an array size of 50,000 elements.
* Naive ($O(n^2)$) Algorithm : Averaged 1.760683 seconds across three runs (1.770652s, 1.738730s, 1.772666s).
* Single-Pass ($O(n)$) Algorithm : Averaged 0.0000853 seconds across three runs (0.000083s, 0.000087s, 0.000086s).
   
The single-pass algorithm outperformed the naive implementation by a factor of approximately 20,634 times.

3. Phase-Level Execution Breakdown:
The instrumented multi-phase dataset experiment (instrumentation_lab.c) yielded a overall total time of 0.001217 seconds, distributed across internal computational phases:
* BUILD_DATA : 0.000490 seconds (~40.26% of total time)
* PROCESS : 0.000488 seconds (~40.10% of total time)
* REDUCE : 0.000233 seconds (~19.15% of total time)

The combined time of the sub-phases (0.001211 seconds) was slightly less than the total measured execution time (0.001217 seconds), confirming a tiny timing measurement overhead of 0.000006 seconds.

Relation Between Runtime and Energy Consumption

CPU energy consumption is directly linked to execution time and active processor state duration. While standard C timing utilities do not measure physical power draw in Watts, active CPU runtime serves as a direct proxy for energy usage under uniform clock frequencies. 

When executing the naive counting algorithm, the CPU was held in a high-power active computing state for ~1.76 seconds per run. Conversely, the optimized single-pass implementation completed the exact same functional work in 0.000085 seconds. By scaling this algorithm across millions of daily server requests, reducing execution time by a factor of 20,634 directly slashes the total energy spent maintaining active CPU cycles, cooling server hardware, and drawing power from the electrical grid.

Furthermore, phase instrumentation reveals that BUILD_DATA and PROCESS consumed over 80% of total runtime due to complex modulo and arithmetic operations. Energy-focused optimization efforts must target these heavy algorithmic phases first to yield the highest reduction in active CPU time.

Limitations of the Experiment

While timing measurements offer a strong foundation for performance analysis, this experiment has several methodological limitations:

1. Proxy Measurement : The clock() function measures CPU time consumed by the process, not direct physical energy (Joules or Watts). It ignores dynamic CPU frequency scaling (DVFS), thermal throttling, and hardware architecture states.
2. Memory Hierarchy Omission : The benchmarks operate on relatively small data structures ($N = 50,000$) residing entirely within CPU cache, failing to model modern memory bus energy bottlenecks or DRAM power consumption.
3. Absence of Optimization Flags : Tests were conducted without compiler optimization (-O2/-O3), which can dramatically alter real-world execution profiles and loop vectorization.

Practical Engineering Takeaway

Energy-efficient software engineering does not begin with complex hardware power profiling; it starts with algorithmic discipline. As demonstrated by the 20,634-fold speedup achieved through algorithmic refactoring, choosing an $O(n)$ algorithm over an $O(n^2)$ approach delivers orders-of-magnitude greater energy savings than micro-optimizations or compiler flags could ever achieve. Software sustainability is primarily driven by efficient algorithmic design and disciplined timing verification.
