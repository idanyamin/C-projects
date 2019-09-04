#ifndef EX2_BATTLESHIPS_H
#define EX2_BATTLESHIPS_H
#endif

//******** -structs- ********************
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
Board *initBoard(int size);

/**
 * prints the board
 * @param board
 */
void printBoard(Board *board);

/**
 * @param uRow user's col
 * @param uCol user's row
 * @param board game board
 * @param ship ships
 * @return 1 if move was valid, 0 otherwise
 */
int bomb(char uRow, int uCol, Board *board, Ship **ship);

/**
 * @param size of board
 * @return an empty board ,NULL if memory allocation went wrong
 */
void freeBoard(Board **board);

/**
 * assign the user's input into boardSize
 * @param boardSize size of board
 * @return 0 if succeed, 1 otherwise.
 */
int getBoardSize(int *boardSize);

/**
 * puts all ships on the game board
 * @param board game board
 * @param shipArr array of ships
 * @return 0 if succeed ,1 otherwise.
 */
Ship **putAllShipsOnBoard(Board *board, Ship **shipArr);

/**
 * get user input
 * @param row row input
 * @param col col input
 * @param board game board
 * @return 0 if input is valid, 1 invalid move, 2 game over
 */
int getUserInput(char *row, int *col, Board *board);


/**
 * free all ships in array
 * @param ship array of ships
 */
void freeAllShips(Ship **ship);
