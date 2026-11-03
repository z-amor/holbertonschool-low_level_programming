# Comparison Algorithms - Metrics Report

## Run 1
Naive algorithm result: 625025000
Naive algorithm time: 1.770652 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000083 seconds

## Run 2
Naive algorithm result: 625025000
Naive algorithm time: 1.738730 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000087 seconds

## Run 3
Naive algorithm result: 625025000
Naive algorithm time: 1.772666 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000086 seconds

## Data Analysis

### Average Execution Times
- **Naive Algorithm:**

  $$
  \text{Average} = \frac{1.770652 + 1.738730 + 1.772666}{3} \approx 1.760683 \text{ seconds}
  $$

- **Single-pass Algorithm:**

  $$
  \text{Average} = \frac{0.000083 + 0.000087 + 0.000086}{3} \approx 0.00008533 \text{ seconds}
  $$

### Relative Difference
- **Ratio calculation:**

  $$
  \text{Ratio} = \frac{1.760683}{0.00008533} \approx 20633.81
  $$

- **Conclusion:** The naive implementation is approximately **20,634 times slower** than the single-pass implementation.