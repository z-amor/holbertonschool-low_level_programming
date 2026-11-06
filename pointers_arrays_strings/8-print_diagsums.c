#include "main.h"
/**
 * print_diagsums - prints the sum of the two diagonals of a square matrix of integers
 * @a: The square matrix of integers
 *
 * Return: void
 */
void print_diagsums(int *a, int size)
{
int sum1 = 0, sum2 = 0;
int i;
for (i = 0; i < size; i++)
{
sum1 += a[i * size + i];
sum2 += a[i * size + (size - 1 - i)];
}
_putchar(sum1);
_putchar(sum2);
}
