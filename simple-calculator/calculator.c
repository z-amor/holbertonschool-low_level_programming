#include <stdio.h>
/**
 * main - Entry point of the program
 *
 * Return: Always 0 (Success)
*/
int main(void)
{
    int choice = -1;
    while(choice != 0)
    {
        printf("Welcome to the Simple Calculator!\nChoose an operation:\n");
        printf("1. Add\n");
        printf("2. Subtract\n");
        printf("3. Multiply\n");
        printf("4. Divide\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
    }
    return (0);
}
