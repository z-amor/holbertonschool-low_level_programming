#include <stdio.h>
/**
* main - Entry point
*
* Return: Always 0 (Success)
*/
int main(void)
{
char a;
char A;
a = 'a';
A = 'A';
while (a <= 'z' && A <= 'Z')
{
putchar(a);
putchar(A);
a++;
A++;
}
putchar('\n');
return (0);
}
