#include <stdio.h>
/**
 * main - Entry point of the program
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
int choice = -1;
int A, B;
while (choice != 0)
{
printf("Welcome to the Simple Calculator!\nChoose an operation:\n");
printf("1. Add\n");
printf("2. Subtract\n");
printf("3. Multiply\n");
printf("4. Divide\n");
printf("0. Exit\n");
printf("Enter your choice: ");
scanf("%d", &choice);
if (choice == 1)
{
printf("A: ");
scanf("%d", &A);
printf("B: ");
scanf("%d", &B);
printf("Result: %d\n", A + B);
}
else if (choice == 2)
{
printf("A: ");
scanf("%d", &A);
printf("B: ");
scanf("%d", &B);
printf("Result: %d\n", A - B);
}
else if (choice == 3)
{
printf("A: ");
scanf("%d", &A);
printf("B: ");
scanf("%d", &B);
printf("Result: %d\n", A * B);
}
else if (choice == 4)
{
printf("A: ");
scanf("%d", &A);
printf("B: ");
scanf("%d", &B);
printf("Result: %d\n", A / B);
}
if (B == 0)
{
printf("Error: Division by zero is not allowed.\n");
}
else if (choice > 4 || choice < 0)
{
printf("Invalid choice\n");
}
else if (choice == 0)
{
printf("Bye!\n");
}
}
return (0);
}
