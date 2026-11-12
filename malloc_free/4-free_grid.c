#include "main.h"
#include <stdlib.h>
/**
 * free_grid - frees a two-dimensional integer grid
 * @grid: pointer to the grid to free
 * @height: number of rows in the grid
 *
 * Return: void
 */
void free_grid(int **grid, int height)
{
int i;
if (grid == NULL || height <= 0)
{
return;
}
for (i = 0; i < height; i++)
{
free(grid[i]);
}
free(grid);
}
