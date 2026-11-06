#include "main.h"
/**
 * print_diagsums - prints the sums of a matrix's diagonals
 * @a: The square matrix of integers
 * @size: The size of the square matrix
 *
 * Return: void
 */
void print_diagsums(int *a, int size)
{
int sums[2] = {0, 0}, i;
unsigned int num, div;
for (i = 0; i < size; i++)
{
sums[0] += a[i * size + i];
sums[1] += a[i * size + size - 1 - i];
}
for (i = 0; i < 2; i++)
{
if (i)
{
_putchar(',');
_putchar(' ');
}
num = sums[i];
if (sums[i] < 0)
{
_putchar('-');
num = -num;
}
for (div = 1; num / div > 9; div *= 10)
;
for (; div; div /= 10)
{
_putchar('0' + num / div);
num %= div;
}
}
_putchar('\n');
}
