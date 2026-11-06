#include "main.h"
/**
 * _strspn - gets the length of a prefix substring
 * @s: The string to search
 * @accept: The prefix substring to search for
 *
 * Return: The number of bytes in the initial segment of s which consist only
 * of bytes from accept
 */
unsigned int _strspn(char *s, char *accept)
{
unsigned int count = 0;
while (*s)
{
char *a = accept;
while (*a)
{
if (*s == *a)
{
count++;
break;
}
a++;
}
if (!*a)
{
break;
}
s++;
}
return (count);
}  
