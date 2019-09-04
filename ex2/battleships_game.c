#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "battleships.h"


const char START_MESSAGE[] = "Ready to play\n";
const char GAME_OVER[] = "Game over\n";
const char MEMORY_ERROR[] = "Memory allocation went wrong";

/**
 * managing game logic
 * @return 0 if the program was running without errors, 1 otherwise
 */
int main()
{
    // game logic
    Board *board = NULL;
    Ship **shipArr = NULL;
    int boardSize = 0;
    printf(START_MESSAGE);
    // if board size doesn't fit
    if (getBoardSize(&boardSize) == 1)
    {
        return 1;
    }
    board = initBoard(boardSize);
    // memory allocation went wrong
    if (board == NULL)
    {
        fprintf(stderr, MEMORY_ERROR);
        return 1;
    }
    shipArr = putAllShipsOnBoard(board, shipArr);
    // memory allocation went wrong
    if (shipArr == NULL)
    {
        freeBoard(&board);
        fprintf(stderr, MEMORY_ERROR);
        return 1;
    }
    printBoard(board);
    // while there are still ships to bomb
    while (board->health > 0)
    {
        // user row and col
        char uRow;
        int uCol;
        int userInputValidity = getUserInput(&uRow, &uCol, board);
        // demand valid input from user
        while (userInputValidity == 1)
        {
            userInputValidity = getUserInput(&uRow, &uCol, board);
        }
        // getUserInput return 2 if game over
        if (userInputValidity == 2)
        {
            break;
        }
        bomb(uRow, uCol, board, shipArr);
        printBoard(board);
    }
    // free all memory allocation
    printf(GAME_OVER);
    freeBoard(&board);
    freeAllShips(shipArr);
    free(shipArr);
    return 0;
}



