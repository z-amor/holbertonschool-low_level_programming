#include "main.h"
/**
 * rev_string - reverses a string
 * @s: pointer to the string
 *
 * Return: void
 */
void rev_string(char *s)
{
int l = 0;
int i;
char tmp;
if (s == 0)
return;
/* Find the length of the string */
while (s[l] != '\0')
{
l++;
}
/* Reverse the string in place */
for (i = 0; i < l / 2; i++)
{
tmp = s[i];
s[i] = s[l - 1 - i];
s[l - 1 - i] = tmp;
}
}
