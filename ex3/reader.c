/**
 * @author Idan Yamin
 * @brief print the results of how the heat spread on a two-dimensional surface, notice the input file has
 * @brief to be in the same format as in the pdf description, meaning after ',' there must be a whitespace.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"
#include "heat_eqn.h"

#define LINE_LEN 1000
#define ERROR 1
#define SUCCESS 0

const char NUM_SEPERATOR[] = ",";
const char COMMAND_SEPERATOR[] = "----";
// messages
const char FORMAT_ERROR[] = "Bad format\n";
const char MEM_ERR[] = "Memory allocation error\n";
const char OUT_OF_RANGE[] = "Sources out of range\n";
const char FILE_OPENING_ERR[] = "File opening error\n";

/**
 * init all of the entries to zero
 * @param grid the grid
 * @param row number of rows
 * @param col number of cols
 */
void initGridValues(double **grid, size_t row, size_t col);

/**
 * build grid with zeros
 * @param row number of rows of new grid
 * @param col number of cols of new grid
 * @return new grid
 */
double **buildGrid(size_t row, size_t col);

/**
 * put sources on the grid
 * @param grid the grid
 * @param sources sources to insert to the grid
 * @param numOfPoints number of sources
 * @return 1 if points out of range, 0 if succeeded.
 */
int putSources(double **grid, size_t m, size_t n, source_point *sources, size_t numOfPoints);

/**
 * get row and col from user
 * @param row update the row
 * @param col update the col
 * @return 1 if there was an error, 0 if succeed.
 */
int getRowAndCol(size_t *row, size_t *col, FILE *file);

/**
 * update sources and numOfSources
 * @param sources make sure to send NULL
 * @param numOfSources num of sources
 * @param file the file of the input
 * @return 1 of there was an error, 0 if succeeded.
 */
int buildSources(source_point **sources, size_t *numOfSources, FILE *file);

/**
 * free grid
 * @param grid the grid to free
 * @param n rows
 * @param m columns
 */
void freeGrid(double **grid, size_t n);

/**
 * get final section of parameters
 * @param termination put the termination value here
 * @param iterNum  put the iterNum here
 * @param isCyclic  put the isCyclic here
 * @param file the input file
 * @return 0 if succeeded, 1 otherwise
 */
int getFinalParameters(double *termination, unsigned int *iterNum, int *isCyclic, FILE *file);

/**
 * print results
 * @param grid of heat values
 * @param n number of rows
 * @param m number columns
 * @param sources of heat
 * @param num_sources
 * @param terminate terminate threshold
 * @param n_iter number of iter per print
 * @param is_cyclic cyclic or not
 */
void printResults(double **grid, size_t n, size_t m, source_point *sources, size_t num_sources,
                  double terminate, unsigned int n_iter, int is_cyclic);

int main(int argc, char *argv[])
{
    // parameters
    char line[LINE_LEN];
    FILE *file = NULL;
    size_t rowNum = 0, colNum = 0, numOfSources = 0;
    unsigned int iterNum = 0;
    double **grid = NULL, termination = 0;
    int error = 0, isCyclic = 0;
    source_point *sources = NULL;


    //  not the right amount of arguments
    if (argc != 2)
    {
        fprintf(stderr, FILE_OPENING_ERR);
        return ERROR;
    }
    strcpy(line, argv[1]);
    file = fopen(line, "r");
    if (file == NULL)
    {
        fprintf(stderr, FILE_OPENING_ERR);
        fclose(file);
        return ERROR;
    }

    // get rows and columns
    error = getRowAndCol(&rowNum, &colNum, file);
    if (error)
    {
        fprintf(stderr, FORMAT_ERROR);
        fclose(file);
        return ERROR;
    }

    // building a grid in the right size
    grid = buildGrid(rowNum, colNum);
    if (grid == NULL)
    {
        fprintf(stderr, MEM_ERR);
        fclose(file);
        return ERROR;
    }

    // build sources, free grid in case of error
    if (buildSources(&sources, &numOfSources, file) == ERROR)
    {
        freeGrid(grid, rowNum);
        fprintf(stderr, FORMAT_ERROR);
        fclose(file);
        return ERROR;
    }

    // get termination iterNum and isCyclic
    if (getFinalParameters(&termination, &iterNum, &isCyclic, file) == ERROR)
    {
        free(sources);
        freeGrid(grid, rowNum);
        fprintf(stderr, FORMAT_ERROR);
        fclose(file);
        return ERROR;
    }

    //put sources on board
    if (putSources(grid, rowNum, colNum, sources, numOfSources) == ERROR)
    {
        free(sources);
        freeGrid(grid, rowNum);
        fprintf(stderr, OUT_OF_RANGE);
        fclose(file);
        return ERROR;
    }

    // print results
    printResults(grid, rowNum, colNum, sources, numOfSources, termination, iterNum, isCyclic);

    // free all sources
    freeGrid(grid, rowNum);
    free(sources);

    // close file
    fclose(file);
    return SUCCESS;
}

/**
 * print grid
 * @param grid grid of heat values
 * @param n number of rows
 * @param m number of columns
 * @param value heat value
 */
void printGrid(double *const *grid, size_t n, size_t m, double value)
{
    printf("%lf\n", value);
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            printf("%2.4lf,", grid[i][j]);
        }
        printf("\n");
    }
}

/**
 * print results
 * @param grid of heat values
 * @param n number of rows
 * @param m number columns
 * @param sources of heat
 * @param num_sources
 * @param terminate terminate threshold
 * @param n_iter number of iter per print
 * @param is_cyclic cyclic or not
 */
void printResults(double **grid, size_t n, size_t m, source_point *sources, size_t num_sources,
                  double terminate, unsigned int n_iter, int is_cyclic)
{
    double value = calculate(heat_eqn, grid, n, m, sources, num_sources, terminate, n_iter,
                             is_cyclic);
    while (value > terminate)
    {
        printGrid(grid, n, m, value);
        value = calculate(heat_eqn, grid, n, m, sources, num_sources, terminate, n_iter,
                          is_cyclic);
    }
    printGrid(grid, n, m, value);
}

/**
 * get final section of parameters
 * @param termination put the termination value here
 * @param iterNum  put the iterNum here
 * @param isCyclic  put the isCyclic here
 * @param file the input file
 * @return 0 if succeeded, 1 otherwise
 */
int getFinalParameters(double *termination, unsigned int *iterNum, int *isCyclic, FILE *file)
{
    char line[LINE_LEN];
    int errFlag = fscanf(file, "%s", line);
    if (errFlag != 1 || strcmp(line, COMMAND_SEPERATOR + 1) != 0)
    {
        return ERROR;
    }
    if (fscanf(file, "%lf", termination) != 1)
    {
        return ERROR;
    }
    int signedIter = 0;
    if (fscanf(file, "%d", &signedIter) != 1)
    {
        return ERROR;
    }
    if (signedIter < 0)
    {
        *iterNum = 0;
    }
    else
    {
        *iterNum =(unsigned int) signedIter;
    }
    if (fscanf(file, "%d", isCyclic) != 1)
    {
        return ERROR;
    }
    return SUCCESS;
}

/**
 * free grid
 * @param grid the grid to free
 * @param n rows
 * @param m columns
 */
void freeGrid(double **grid, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        free(grid[i]);
    }
    free(grid);
}

/**
 * update sources and numOfSources
 * @param sources make sure to send NULL
 * @param numOfSources num of sources
 * @param file the file of the input
 * @return 1 of there was an error, 0 if succeeded.
 */
int buildSources(source_point **sources, size_t *numOfSources, FILE *file)
{
    *numOfSources = 0;
    int row = 0, col = 0;
    double heat = 0;
    // while you can still read points
    while (fscanf(file, "%d, %d, %lf", &row, &col, &heat) == 3)
    {
        sources[0] = (source_point *) realloc(sources[0],
                                              sizeof(source_point) * ((*numOfSources) + 1));
        if (!(*sources))
        {
            free(sources);
            return ERROR;
        }
        sources[0][*numOfSources].x = row;
        sources[0][*numOfSources].y = col;
        sources[0][*numOfSources].value = heat;
        (*numOfSources)++;
    }

    return SUCCESS;
}

/**
 * get row and col from user
 * @param row update the row
 * @param col update the col
 * @return 1 if there was an error, 0 if succeed.
 */
int getRowAndCol(size_t *row, size_t *col, FILE *file)
{
    int errorFlag = 0;
    char str[LINE_LEN] = "";
    // try to get first number
    errorFlag = fscanf(file, "%lu, %lu", row, col);
    if (!errorFlag)
    {
        return ERROR;
    }
    // expected ---- after the the size of the board
    fscanf(file, "%s", str);
    if (strcmp(str, COMMAND_SEPERATOR) != 0)
    {
        return ERROR;
    }
    return SUCCESS;
}

/**
 * put sources on the grid
 * @param grid the grid
 * @param sources sources to insert to the grid
 * @param numOfPoints number of sources
 * @return 1 if points out of range, 0 if succeeded.
 */
int putSources(double **grid, size_t m, size_t n, source_point *sources, size_t numOfPoints)
{
    for (size_t i = 0; i < numOfPoints; i++)
    {
        if ((size_t)sources[i].x >= m || (size_t)sources[i].x < 0 || (size_t)sources[i].y >= n ||
            (size_t)sources[i].y < 0)
        {
            return ERROR;
        }
        grid[sources[i].x][sources[i].y] = sources[i].value;
    }
    return SUCCESS;
}

/**
 * build grid with zeros
 * @param row number of rows of new grid
 * @param col number of cols of new grid
 * @return new grid
 */
double **buildGrid(size_t row, size_t col)
{
    double **newGrid = malloc(sizeof(int **) * row);
    if (!newGrid)
    {
        return NULL;
    }
    for (size_t i = 0; i < row; i++)
    {
        newGrid[i] = malloc(sizeof(int *) * col);
        // in case of an allocation error free all memory
        if (!newGrid[i])
        {
            for (size_t j = 0; j < i; j++)
            {
                free(newGrid[i]);
            }
            free(newGrid);
            return NULL;
        }
    }
    initGridValues(newGrid, row, col);
    return newGrid;
}

/**
 * init all of the entries to zero
 * @param grid the grid
 * @param row number of rows
 * @param col number of cols
 */
void initGridValues(double **grid, size_t row, size_t col)
{
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            grid[i][j] = 0;
        }
    }
}
