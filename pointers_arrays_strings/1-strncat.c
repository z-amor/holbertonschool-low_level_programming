#include "main.h"
/**
 * _strncat - Concatenates up to n characters from src to dest.
 * @dest: The destination string to append to.
 * @src: The source string to append.
 * @n: The maximum number of characters to append from src.
 * Return: A pointer to the destination string.
 */
char *_strncat(char *dest, char *src, int n)
{
int i = 0;
int j = 0;
/* Find the end of the dest string */
while (dest[i] != '\0')
{
i++;
}
/* Append src characters to dest */
while (src[j] != '\0' && j < n)
{
dest[i] = src[j];
i++;
j++;
}
/* Add the terminating null byte */
dest[i] = '\0';
return (dest);
}
