/**
 * @author Idan Yamin
 */

#include <stdio.h>
#include "calculator.h"


// ____________ functions _______________
int isSource(size_t row, size_t col, const source_point *source,
             size_t num_sources);

double calcHeat(double **grid, size_t n, size_t m);

void updateGrid(diff_func function, double **grid, size_t n, size_t m,
                const source_point *sources, size_t num_sources, int is_cyclic);

double getLeft(size_t row, size_t col, double **grid, size_t m, int is_cyclic);

double getRight(size_t row, size_t col, double **grid, size_t m, int is_cyclic);

double getBottom(size_t row, size_t col, double **grid, size_t n, int is_cyclic);

double getTop(size_t row, size_t col, double **grid, size_t n, int is_cyclic);


/**
* Calculator function. Applies the given function to every point in the grid iteratively for n_iter loops, or until the cumulative difference is below terminate (if n_iter is 0).
*/
double calculate(diff_func function, double **grid, size_t n, size_t m, source_point *sources,
                 size_t num_sources, double terminate, unsigned int n_iter, int is_cyclic)
{
    double diff = 0;
    // if iter > 0 than run until n_iter
    if (n_iter > 0)
    {
        for (unsigned int i = 0; i < n_iter; i++)
        {
            double prevSum = calcHeat(grid, n, m);
            updateGrid(function, grid, n, m, sources, num_sources, is_cyclic);
            double currSum = calcHeat(grid, n, m);
            diff = currSum - prevSum;
            if (diff < 0)
            {
                diff *= -1;
            }
        }
    }
    else
    {
        // run until diff < terminate
        do
        {
            double prevSum = calcHeat(grid, n, m);
            updateGrid(function, grid, n, m, sources, num_sources, is_cyclic);
            double currSum = calcHeat(grid, n, m);
            diff = currSum - prevSum;
            if (diff < 0)
            {
                diff *= -1;
            }
        } while (diff >= terminate);
    }
    return diff;
}

/**
 * calculate the sum of the heat
 * @param grid of heat
 * @param n num of rows
 * @param m num columns
 * @return the sum of the values of the grid
 */
double calcHeat(double **grid, const size_t n, const size_t m)
{
    double sum = 0;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            sum += *(*(grid + i) + j);
        }
    }
    return sum;
}

/**
 * @param row coordinate of grid.
 * @param col coordinate of grid.
 * @param source a pointer to an array of all sources.
 * @param num_sources the num of sources.
 * @return 1 if flag is source, 0 if flag isn't source.
 */
int isSource(const size_t row, const size_t col, const source_point *source,
             const size_t num_sources)
{
    int flag = 0;
    for (size_t i = 0; i < num_sources; i++)
    {
        // if the source fits the given coordinates
        if ((size_t)(source + i)->x == row && (size_t)(source + i)->y == col)
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

/**
 * calculate the new grid
 * @param function the given function
 * @param grid grid of values
 * @param n number of rows
 * @param m number of columns
 * @param sources the sources of heat
 * @param num_sources the number of sources
 * @param is_cyclic 1 for cyclic, 0 for not cyclic.
 */
void updateGrid(const diff_func function, double **grid, const size_t n, const size_t m,
                const source_point *sources, const size_t num_sources, const int is_cyclic)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            if (!isSource(i, j, sources, num_sources))
            {
                double left = getLeft(i, j, grid, m, is_cyclic);
                double right = getRight(i, j, grid, m, is_cyclic);
                double bottom = getBottom(i, j, grid, n, is_cyclic);
                double top = getTop(i, j, grid, n, is_cyclic);
                grid[i][j] = function(grid[i][j], right, top, left, bottom);
            }
        }
    }
}

/**
 *
 * @param row in the grid
 * @param col int rhe grid
 * @param grid heat grid
 * @param m rows
 * @param is_cyclic is grid cyclic or not
 * @return the left value to the given coordinates
 */
double getLeft(const size_t row, size_t col, double **grid,
               const size_t m, const int is_cyclic)
{
    if (col == 0)
    {
        if (!is_cyclic)
        {
            return 0;
        }
        col = m - 1;
    }
    else
    {
        col--;
    }
    return grid[row][col];

}

/**
 *
 * @param row in the grid
 * @param col int rhe grid
 * @param grid heat grid
 * @param m rows
 * @param is_cyclic is grid cyclic or not
 * @return the right value to the given coordinates
 */
double getRight(const size_t row, size_t col, double **grid, const size_t m,
                const int is_cyclic)
{
    if (col + 1 == m)
    {
        if (!is_cyclic)
        {
            return 0;
        }
        col = 0;
    }
    else
    {
        col++;
    }
    return grid[row][col];

}

/**
 *
 * @param row in the grid
 * @param col int rhe grid
 * @param grid heat grid
 * @param n columns
 * @param is_cyclic is grid cyclic or not
 * @return the bottom value to the given coordinates
 */
double getBottom(size_t row, const size_t col, double **grid, const size_t n, const int is_cyclic)
{
    if (row == 0)
    {
        if (!is_cyclic)
        {
            return 0;
        }
        row = n - 1;
    }
    else
    {
        row--;
    }
    return grid[row][col];
}

/**
 *
 * @param row in the grid
 * @param col int rhe grid
 * @param grid heat grid
 * @param n columns
 * @param is_cyclic is grid cyclic or not
 * @return the top value to the given coordinates
 */
double getTop(size_t row, size_t col, double **grid, size_t n, int is_cyclic)
{
    if (row + 1 == n)
    {
        if (!is_cyclic)
        {
            return 0;
        }
        row = 0;
    }
    else
    {
        row++;
    }
    return grid[row][col];
}
