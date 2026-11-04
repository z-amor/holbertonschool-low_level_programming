#include "main.h"
/**
 * puts_half - prints half of a string
 * @str: pointer to the string
 *
 * Return: void
 */
void puts_half(char *str)
{
int i;
int l = 0;
while (str[l] != '\0')
{
l++;
}
for (i = l / 2; str[i] != '\0'; i++)
{
_putchar(str[i]);
}
_putchar('\n');
}
