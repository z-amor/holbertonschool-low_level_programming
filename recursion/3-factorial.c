#include "main.h"
/**
 * factorial - Returns the factorial of a number
 * @n: The number whose factorial is to be returned
 *
 * Return: The factorial of the number
 */
int factorial(int n)
{
if (n < 0)
return (-1);
if (n == 0)
return (1);
return (n * factorial(n - 1));
}
