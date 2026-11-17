#include "3-calc.h"
/**
 * main - check the code for the calculator
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: Always 0 on success, exits with specific status on error.
 */
int main(int argc, char *argv[])
{
int num1, num2, result;
int (*op_func)(int, int);
if (argc != 4)
{
printf("Error\n");
exit(98);
}
op_func = get_op_func(argv[2]);
if (op_func == NULL || argv[2][1] != '\0')
{
printf("Error\n");
exit(99);
}
if ((*argv[2] == '/' || *argv[2] == '%') && atoi(argv[3]) == 0)
{
printf("Error\n");
exit(100);
}
num1 = atoi(argv[1]);
num2 = atoi(argv[3]);
result = op_func(num1, num2);
printf("%d\n", result);
return (0);
}
