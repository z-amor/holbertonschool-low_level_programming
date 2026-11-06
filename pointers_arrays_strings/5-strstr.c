#include "main.h"
/**
 * _strstr - locates a substring in a string
 * @haystack: The string to search
 * @needle: The substring to search for
 *
 * Return: A pointer to the beginning of the located substring, or NULL if not found
 */
char *_strstr(char *haystack, char *needle)
{
if (*needle == '\0')
{
return (haystack);
}
while (*haystack)
{
char *h = haystack;
char *n = needle;
while (*n && *h == *n)
{
h++;
n++;
}
if (!*n)
{
return (haystack);
}
haystack++;
}
return (NULL);
}
