#include "main.h"
/**
 * swap_int - swaps the values of two integers
 * @a : First integer pointer to swap
 * @b : Second integer pointer to swap
 *
 * Return: Void
 */
void swap_int(int *a, int *b)
{
int tmp;
tmp = *a;
*a = *b;
*b = tmp;
}
