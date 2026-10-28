#include <stdio.h>
/**
* main - Entry point
*
* Return: Always 0 (Success)
*/
int main(void)
{
int n;
char a;
n = 0;
while (n < 10)
{
putchar(n + '0');
n++;
}
a = 'a';
while (a <= 'f')
{
putchar(a);
a++;
}
putchar('\n');
return (0);
}
