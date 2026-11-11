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
unsigned int i, len;
if (str == NULL)
{
return (NULL);
}
/* Calcul de la longueur de la chaîne */
for (len = 0; str[len] != '\0'; len++)
{
/* Allocation de la mémoire (+1 pour le caractère '\0') */
dup = malloc(sizeof(char) * (len + 1));
}
if (dup == NULL)
{
return (NULL);
}
/* Copie de la chaîne (incluant '\0')*/
for (i = 0; i <= len; i++)
{
dup[i] = str[i];
}
return (dup);
}
