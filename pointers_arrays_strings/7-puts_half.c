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
/* On calcule l'indice de départ correct pour pair et impair */
i = l - (l / 2);
for (i = l / 2; str[i] != '\0'; i++)
{
_putchar(str[i]);
}
_putchar('\n');
}
