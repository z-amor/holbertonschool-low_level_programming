#include "main.h"
/**
 * cap_string - Capitalizes the first letter of each word in a string
 * @str: The string to modify
 *
 * Return: A pointer to the modified string
 */
char *cap_string(char *str)
{
int i;
for (i = 0; str[i] != '\0'; i++)
{
if (i == 0 || str[i - 1] == ' ')
{
if (str[i] >= 'a' && str[i] <= 'z')
{
str[i] -= 32;
}
}
}
return (str);
}
