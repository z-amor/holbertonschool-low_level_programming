#include "main.h"
/**
 * print_rev - prints a string in reverse
 * @s: pointer to the string
 *
 * Return: void
 */
void print_rev(char *s)
{
int l = 0;
if (s == 0)
return;
/* Find the length of the string */
while (s[l] != '\0')
{
l++;
}
/* Print characters from the end to the beginning */
for (l -= 1; l >= 0; l--)
{
_putchar(s[l]);
}
_putchar('\n');
}
