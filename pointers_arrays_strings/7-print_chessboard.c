#include "main.h"
/**
 * print_chessboard - prints the chessboard
 * @a: The chessboard to print
 *
 * Return: void
 */
void print_chessboard(char (*a)[8])
{
int row;
int col;
row = 0;
while (row < 8)
{
col = 0;
while (col < 8)
{
_putchar(a[row][col]);
col++;
}
_putchar('\n');
row++;
}
}
int main(void)
{
char board[8][8] = {
{'r', 'k', 'b', 'q', 'k', 'b', 'k', 'r'},
{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
{'R', 'K', 'B', 'Q', 'K', 'B', 'K', 'R'},
};
print_chessboard(board);
return (0);
}
