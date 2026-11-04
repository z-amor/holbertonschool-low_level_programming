#include "main.h"
/**
 * print_array - prints n elements of an array of integers, followed by a new line.
 * @a: pointer to the array
 * @n: number of elements in the array
 */
void print_array(int *a, int n)
{
int i, div, num;
for (i = 0; i < n; i++)
{
num = a[i];
/* 1. On gère d'abord le séparateur entre les éléments */
if (i != 0)
{
_putchar(',');
}
/* 2. Ensuite on affiche le signe moins si le nombre est négatif */
if (num < 0)
{
_putchar('-');
}
/* 3. Enfin, on affiche la valeur numérique */
if (num == 0)
{
_putchar('0');
}
else
{
unsigned int nbr;
if (num < 0)
nbr = (unsigned int)(-num);
else
nbr = (unsigned int)num;
div = 1;
while (nbr / div > 9)
{
div *= 10;
}
while (div != 0)
{
_putchar('0' + (nbr / div));
nbr %= div;
div /= 10;
}
}
}
_putchar('\n');
}
