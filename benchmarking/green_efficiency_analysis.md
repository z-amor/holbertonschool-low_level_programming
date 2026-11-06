# Green Efficiency Analysis Report

## Measurement Methodology

To study how efficiently code runs, experiments were conducted using three C programs in a Linux environment. All programs were compiled with strict flags (`-Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-long-long`) without compiler optimizations. This allowed us to measure the raw efficiency of the code algorithms.

Execution times were measured using the standard C library `<time.h>` and the `clock()` function. The raw time ticks were converted into seconds by dividing them by `CLOCKS_PER_SEC`. To reduce the effect of background computer tasks and variations in CPU scheduling, each test was run multiple times to calculate clear average times.

Timing markers were placed around loops and computational steps. In the multi-phase experiment, timers were recorded before and after each section (`BUILD_DATA`, `PROCESS`, `REDUCE`) and around the full execution (`TOTAL`) to check for accuracy and timing overhead.

## Observed Performance Differences

The measurements showed major differences in execution speed depending on how the code was written:

1. **Baseline Loop Performance:**
   Running a basic loop of 100 million iterations (`baseline_loop.c`) three times produced runtimes of 0.176477s, 0.185758s, and 0.176340s. This shows a very small timing variation (\~9.4 ms) with an average execution time of **0.179525 seconds**.

2. **Algorithm Comparison:**
   The comparison program (`comparison_algorithms.c`) tested two ways of counting numbers in an array of 50,000 items:

   * **Naive Algorithm:** Averaged **1.760683 seconds** across three runs (1.770652s, 1.738730s, and 1.772666s).

   * **Single-Pass Algorithm:** Averaged **0.0000853 seconds** across three runs (0.000083s, 0.000087s, and 0.000086s).

   The single-pass algorithm ran about **20,634 times faster** than the naive algorithm.

3. **Phase-Level Execution Breakdown:**
   The instrumented code (`instrumentation_lab.c`) finished in a total time of **0.001217 seconds**, divided as follows:

   * **BUILD_DATA:** 0.000490 seconds (\~40.26% of total time)

   * **PROCESS:** 0.000488 seconds (\~40.10% of total time)

   * **REDUCE:** 0.000233 seconds (\~19.15% of total time)

   Adding the three sub-phases gives 0.001211 seconds, leaving a tiny difference of 0.000006 seconds taken by the timing overhead itself.

## Relation Between Runtime and Energy Consumption

CPU energy usage is directly connected to execution time. While the `clock()` function does not measure electrical power in Watts, how long a processor stays active is a practical indicator of energy use.

Running the naive algorithm keeps the CPU working hard for about 1.76 seconds per run. The single-pass code completes the exact same task in only 0.000085 seconds. When scaled to millions of operations on servers, running code 20,634 times faster directly lowers electricity use and heating.

The phase measurements also show that `BUILD_DATA` and `PROCESS` take over 80% of the total runtime due to extra math steps. Optimization efforts should focus on these heavy parts first to save energy effectively.

## Limitations of the Experiment

This measurement method has a few limitations:

1. **Indirect Measure:** The `clock()` function measures CPU time, not real physical energy (Joules or Watts). It does not track changes in processor clock speed or power states.

2. **Small Memory Size:** The test array ($N = 50,000$) fits inside the CPU cache, so it does not measure energy used when reading from main RAM memory.

3. **No Optimization Flags:** The programs were compiled without optimization flags (`-O2` or `-O3`), which standard production software normally uses.

## Practical Engineering Takeaway

Writing energy-efficient software depends mostly on good algorithm choices. As shown by the 20,634-times speed improvement, choosing a simpler algorithm saves far more energy than small code tweaks. Sustainable software development starts with clean design and clear measurement.