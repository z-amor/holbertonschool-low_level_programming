#include "main.h"
#include <stdlib.h>
/**
 * _strdup - Returns a pointer to a newly allocated space in memory,
 *           which contains a copy of the string given as a parameter.
 * @str: The string to duplicate
 *
 * Return: Pointer to the duplicated string, or NULL if str is NULL
 *         or if memory allocation fails.
 */
char *_strdup(char *str)
{
char *dup;
int len, i;
if (str == NULL)
{
return (NULL);
}
/* Calculate the length of the string */
len = 0;
while (str[len] != '\0')
{
len++;
}
/* Allocate memory for the duplicate string */
dup = malloc((len + 1) * sizeof(char));
if (dup == NULL)
{
return (NULL);
}
/* Copy the string to the newly allocated memory */
for (i = 0; i < len; i++)
{
dup[i] = str[i];
}
dup[len] = '\0'; 
return (dup);
}
