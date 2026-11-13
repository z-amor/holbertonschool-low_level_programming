#include <stdlib.h>
#include "dog.h"
/**
 * _strlen - Returns the length of a string
 * @s: String to evaluate
 *
 * Return: Length of the string
 */
int _strlen(char *s)
{
int i = 0;
while (s[i])
i++;
return (i);
}
/**
 * _strcpy - Copies a string pointed to by src
 * @dest: Destination buffer
 * @src: Source string
 *
 * Return: Pointer to dest
 */
char *_strcpy(char *dest, char *src)
{
int i = 0;
while (src[i])
{
dest[i] = src[i];
i++;
}
dest[i] = '\0';
return (dest);
}
/**
 * new_dog - Creates a new dog
 * @name: Name of the dog
 * @age: Age of the dog
 * @owner: Owner of the dog
 *
 * Return: Pointer to the new dog, or NULL if it fails
 */
dog_t *new_dog(char *name, float age, char *owner)
{
dog_t *d;
char *name_copy, *owner_copy;
if (name == NULL || owner == NULL)
return (NULL);
d = malloc(sizeof(dog_t));
if (d == NULL)
return (NULL);
name_copy = malloc(_strlen(name) + 1);
if (name_copy == NULL)
{
free(d);
return (NULL);
}
owner_copy = malloc(_strlen(owner) + 1);
if (owner_copy == NULL)
{
free(name_copy);
free(d);
return (NULL);
}
_strcpy(name_copy, name);
_strcpy(owner_copy, owner);
d->name = name_copy;
d->age = age;
d->owner = owner_copy;
return (d);
}
