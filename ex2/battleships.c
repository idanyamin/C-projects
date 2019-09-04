#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

const int VERTICAL = 1;
const int HORIZNTAL = 0;
const int NUM_OF_ORIENTATIONS = 2;


//length of different kinds of ships
const int LENGTH_OF_SUBMARINE = 3;
const int LENGTH_OF_AIRCRAFT_CARRIER = 5;
const int LENGTH_OF_MISSILE_BOAT = 3;
const int LENGTH_OF_DESTROYER = 2;
const int NUM_OF_SHIPS = 4;

const int MAX_BOARD_SIZE = 26;
const int MIN_BOARD_SIZE = 5;
// board table constants
#define EMPTY (0)
#define TAKEN (1)
#define HIT (2)
#define MISS (3)


// display board
const char D_EMPTY = '_';
const char D_MISS = '0';
const char D_HIT = 'x';

// messages
const char MISS_MESSAGE[] = "Miss\n";
const char HIT_MESSAGE[] = "Hit\n";
const char INVALID_MOVE[] = "Invalid move, try again\n";
const char BEEN_HIT[] = "Already been hit.\n";
const char SUNK_MESSAGE[] = "Hit and sunk.\n";
const char ENTER_SIZE[] = "enter board size: ";
const char ENTER_COORDINATES[] = "enter coordinates: ";
const char EXIT[] = "exit";

//errors
const char BOARD_SIZE_ERROR[] = "INVALID BOARD SIZE";

// --------Ship---------------

/**
 * coordinate, contains row and col
 */
typedef struct Coordinate
{
    int row;
    int col;
} Coordinate;
/**
 * Ship, each ship has orientation and length
 */
typedef struct Ship
{
    Coordinate *coordinate;
    int orientation;
    int length;
} Ship;

/**
 * board, contains a matrix representation of board.
 * health (how many hits to win the game)
 * size the size of the board
 */
typedef struct Board
{
    int **boardTable;
    int health;
    int size;
} Board;

/**
 * @param size of board
 * @return an empty board ,NULL if memory allocation went wrong
 */
void freeBoard(Board **board);

/**
 * init a submarine with a given orientation
 * @param orientation VERTICAL or HORIZENTAL
 * @return new submarine, NULL if memory allocation went wrong
 */
Ship *
initShip(const int orientation, const int lengthOfShip, Coordinate *location, const Board *board)
{
    Ship *newShip = (Ship *) malloc(sizeof(Ship));
    //assert(newShip != NULL);
    if (newShip == NULL)
    {
        return NULL;
    }

    newShip->coordinate = location;
    newShip->length = lengthOfShip;
    newShip->orientation = orientation;
    // put the ship on the board
    if (orientation == HORIZNTAL)
    {
        for (int i = location->col; i < location->col + lengthOfShip; i++)
        {
            board->boardTable[location->row][i] = TAKEN;
        }
    }
    else if (orientation == VERTICAL)
    {
        for (int i = location->row; i < location->row + lengthOfShip; i++)
        {
            board->boardTable[i][location->col] = TAKEN;
        }
    }
    return newShip;
}

/**
 * free ship from memory
 * @param ship ship to free from memory
 */
void freeShip(Ship *ship)
{
    if (ship != NULL)
    {
        if ((ship)->coordinate != NULL)
        {
            free((ship)->coordinate);
        }
        // todo check that on linux
        // if think it needs to work because i'm freeing memory that i have allocated
        free(ship);
    }
}


/**
 * @param size of board
 * @return an empty board ,NULL if memory allocation went wrong
 */
Board *initBoard(const int size)
{
    int health = LENGTH_OF_AIRCRAFT_CARRIER + LENGTH_OF_SUBMARINE + LENGTH_OF_MISSILE_BOAT +
                 LENGTH_OF_DESTROYER;
    // create a new board
    Board *board = (Board *) malloc(sizeof(Board));
    //assert(board != NULL);
    if (board == NULL)
    {
        return NULL;
    }
    // create a new table to store the board
    int **boardTable;
    boardTable = (int **) malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++)
    {
        boardTable[i] = malloc(sizeof(int) * size);
        //assert(boardTable[i] != NULL);
        if (boardTable[i] == NULL)
        {
            freeBoard(&board);
            return NULL;
        }
        for (int j = 0; j < size; j++)
        {
            boardTable[i][j] = EMPTY;
        }
    }
    board->boardTable = boardTable;
    board->health = health;
    board->size = size;
    return board;
}

/**
 * free board memory allocation
 * @param board game board
 */
void freeBoard(Board **board)
{
    if (board != NULL && *board != NULL)
    {
        // free all columns
        for (int i = 0; i < (*board)->size; i++)
        {
            if ((*board) != NULL)
            {
                if ((*board)->boardTable[i] != NULL)
                {
                    free((*board)->boardTable[i]);
                }
                (*board)->boardTable[i] = NULL;
            }
        }
        if (*board != NULL)
        {
            // free the board table;
            free((*board)->boardTable);
            (*board)->boardTable = NULL;
            // free the board
            free(*board);
        }
        *board = NULL;
    }
}

/**
 * prints the board
 * @param board
 */
void printBoard(Board *board)
{
    // print a line of numbers
    for (int i = 0; i < board->size; i++)
    {
        printf(",%d", i + 1);
    }
    // print the whole board
    for (int i = 0; i < board->size; i++)
    {
        // print a line of the board
        printf("\n%c", i + 'a');
        for (int j = 0; j < board->size; j++)
        {
            /*
            //todo delete the first if, it's for debug
            if (board->boardTable[i][j] == TAKEN)
            {
                printf("%c ", '*');
            }*/
            // print empty spot on board
            if (board->boardTable[i][j] == EMPTY || board->boardTable[i][j] == TAKEN)
            {
                printf("%c ", D_EMPTY);
            }
            else if (board->boardTable[i][j] == MISS)
            {
                printf("%c ", D_MISS);
            }
            else if (board->boardTable[i][j] == HIT)
            {
                printf("%c ", D_HIT);
            }
        }
    }
    printf("\n");
}

/**
 * put a ship on the board
 * @param board game board
 * @param shipSize of ship
 * @return the ship that was placed on the board, NULL if memory allocation went wrong
 */
Ship *putShipOnBoard(Board *board, const int shipSize)
{
    int maxRange = board->size - shipSize;
    int modulo = maxRange + 1;
    srand(time(NULL));
    int orientation = 0, col = 0, row = 0;
    // take a random coordination till success
    do
    {
        orientation = rand() % NUM_OF_ORIENTATIONS;
        int flag = 1;
        // take care of horizental orientation
        if (orientation == HORIZNTAL)
        {
            row = rand() % board->size;
            col = rand() % modulo;

            for (int i = col; i < col + shipSize; i++)
            {
                if (board->boardTable[row][i] != EMPTY)
                {
                    flag = 0;
                }
            }
            if (flag == 1)
            {
                break;
            }
        }
            // take care of vertical orientation
        else if (orientation == VERTICAL)
        {
            row = rand() % modulo;
            col = rand() % board->size;

            for (int i = row; i < row + shipSize; i++)
            {
                if (board->boardTable[i][col] != EMPTY)
                {
                    flag = 0;
                }
            }
            if (flag == 1)
            {
                break;
            }
        }

    } while (1);
    Coordinate *coordinate = (Coordinate *) malloc(sizeof(Coordinate));
    //assert(coordinate != NULL);
    if (coordinate == NULL)
    {
        return NULL;
    }
    coordinate->col = col;
    coordinate->row = row;
    return initShip(orientation, shipSize, coordinate, board);
}

/**
 * converts the user's coordinate to board coordinate
 * @param row the row to convert
 * @param col the column to convert
 */
void convertUserCoordinateToBoard(int *row, int *col)
{
    *row = *row - 'a';
    *col = *col - 1;
}

/**
 * prints a message in case the ship has been bombed
 * @param bRow the row coordinate of boming
 * @param bCol the col coordiante of boming
 * @param board the game board
 * @param ship an array of pointers to ships
 * @return 1 if a ship has been sunk, 0 otherwise
 */
int isSunk(const int bRow, const int bCol, Board *board, Ship **ship)
{
    for (int i = 0; i < NUM_OF_SHIPS; i++)
    {
        int isSunk = 1;
        int shipRow = (*(ship + i))->coordinate->row;
        int shipCol = (*(ship + i))->coordinate->col;
        if ((*(ship + i))->orientation == HORIZNTAL)
        {
            // if ship was bombed
            if (bRow == shipRow && (shipCol <= bCol && bCol < shipCol + (*(ship + i))->length))
            {
                for (int j = shipCol; j < shipCol + (*(ship + i))->length; j++)
                {
                    int state = board->boardTable[shipRow][j];
                    // the ship wasn't sunk
                    if (state != HIT)
                    {
                        isSunk = 0;
                        break;
                    }
                }
                if (isSunk == 1)
                {
                    return 0;
                }
            }
        }
        else if ((*(ship + i))->orientation == VERTICAL)
        {
            // the ship was bombed
            if (bCol == shipCol && (shipRow <= bRow && bRow < shipRow + (*(ship + i))->length))
            {
                for (int j = shipRow; j < shipRow + (*(ship + i))->length; j++)
                {
                    int state = board->boardTable[j][shipCol];
                    if (state != HIT)
                    {
                        isSunk = 0;
                        break;
                    }
                }
                if (isSunk == 1)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}


/**
 *
 * @param uRow user's col
 * @param uCol user's row
 * @param board game board
 * @param ship ships
 * @return 1 if move was valid, 0 otherwise
 */
int bomb(const char uRow, const int uCol, Board *board, Ship **ship)
{
    // board row and board col
    int bRow = (int) uRow;
    int bCol = (int) uCol;
    convertUserCoordinateToBoard(&bRow, &bCol);
    switch (board->boardTable[bRow][bCol])
    {
        case EMPTY:
            board->boardTable[bRow][bCol] = MISS;
            printf(MISS_MESSAGE);
            return 1;
        case MISS:
            printf(INVALID_MOVE);
            return 0;
        case HIT:
            printf(BEEN_HIT);
            return 0;
        case TAKEN:
            board->boardTable[bRow][bCol] = HIT;
            board->health--;
            if (isSunk(bRow, bCol, board, ship) == 0)
            {
                printf(SUNK_MESSAGE);
            }
            else
            {
                printf(HIT_MESSAGE);
            }
            return 1;
        default:
            return 0;
    }
}

/**
 * free all ships in array
 * @param ship array of ships
 */
void freeAllShips(Ship **ship)
{
    //todo maybe the memory problem is from here
    for (int i = 0; i < NUM_OF_SHIPS; i++)
    {
        freeShip(*(ship + i));
        *(ship + i) = NULL;
    }
}

/**
 * assign the user's input into boardSize
 * @param boardSize size of board
 * @return 0 if succeed, 1 otherwise.
 */

int getBoardSize(int *boardSize)
{
    printf(ENTER_SIZE);
    if (scanf("%d", boardSize) == 0)
    {
        printf("\n");
        fprintf(stderr, BOARD_SIZE_ERROR);
        return 1;
    }
    // not in range
    if (MIN_BOARD_SIZE > *boardSize || *boardSize > MAX_BOARD_SIZE)
    {
        printf("\n");
        fprintf(stderr, BOARD_SIZE_ERROR);
        return 1;
    }
    return 0;
}

/**
 * get user input
 * @param row row input
 * @param col col input
 * @param board game board
 * @return 0 if input is valid, 1 invalid move, 2 game over
 */
int getUserInput(char *row, int *col, Board *board)
{
    printf(ENTER_COORDINATES);
    int length = 0;
    char str[strlen(EXIT) + 1];
    scanf("%s%n", str, &length);

    // game over
    if (strcmp(str, EXIT) == 0)
    {
        return 2;
    }
    if (strlen(str) == 1)
    {
        *row = str[0];
        if (scanf("%d", &*col) == 1)
        {
            if (0 > *row - 'a' || *row - 'a' + 1 > board->size || *col < 1 || *col > board->size)
            {
                printf("\n");
                fprintf(stderr, INVALID_MOVE);
                return 1;
            }
            return 0;
        }
    }
    printf("\n");
    fprintf(stderr, INVALID_MOVE);
    return 1;

}

/**
 * puts all ships on the game board
 * @param board game board
 * @param shipArr array of ships
 * @return 0 if succeed ,1 otherwise.
 */
Ship **putAllShipsOnBoard(Board *board, Ship **shipArr)
{
    shipArr = (Ship **) malloc(sizeof(Ship *) * NUM_OF_SHIPS);
    if (shipArr == NULL)
    {
        return NULL;
    }
    *(shipArr) = putShipOnBoard(board, LENGTH_OF_AIRCRAFT_CARRIER);
    *(shipArr + 1) = putShipOnBoard(board, LENGTH_OF_MISSILE_BOAT);
    *(shipArr + 2) = putShipOnBoard(board, LENGTH_OF_SUBMARINE);
    *(shipArr + 3) = putShipOnBoard(board, LENGTH_OF_DESTROYER);
    if ((*shipArr) == NULL || *(shipArr + 1) == NULL || *(shipArr + 2) == NULL
        || *(shipArr + 3) == NULL)
    {
        for (int i = 0; i < NUM_OF_SHIPS; i++)
        {
            free(*(i + shipArr));
            *(shipArr + i) = NULL;
        }
        free(shipArr);
        return NULL;
    }
    return shipArr;
}
