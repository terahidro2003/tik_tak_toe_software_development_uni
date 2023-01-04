#include <stdio.h>
#include <stdbool.h>

/*
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * @Program Tik-Tak-Toe Game in C language
 * @Assessment Week 9/10 | C Programming Assessment
 * @Due December 14, 2022 – 23:59 CET (via Moodle)
 * @By Juozas Skarbalius (j.skarbalius@lancaster.ac.uk)
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

/*
 * Compliance with requirements
 * 1. The program allows two human-players to play tik-tak-toe game in nxn grid (2<=N<=50) //CHECKED
 * 2. Grid size n is entered through terminal //CHECKED
 * 3. After each mark is set, the grid is printed with updated marks //CHECKED
 * 4. Implementation of row_covered() function //CHECKED
 * 5. Implementation of case when game ends in draw (bool stalemate(char grid[N][N])) //CHECKED
 * 6. Program written in clean way //CHECKED
 * 7. Program executes without any issues //CHECKED
*/

/*
 * Issues confronted:
 * 1. Next player can override other player's move ----- FIXED TESTED
 * 2. Check for top right to bottom left do not work properly ----- FIXED TESTED
 * 3. Segmentation error on printf('\n') due to NULL char pointers. Workaround found putchar('\n') since printf('\n')
 * implementation in most compilers are the same as putchar() at assembly level. ----- FIXED TESTED
 */

/*
INT N
Defines: row and column count, N*N total places for marks
Declared as global variable to make accessing multidimensional array grid easier through functions
*/
int N;

/*
 * @Arg: none
 * @Return type: void
 * @Returns: nothing
 * Asks user to enter board size, reads user's input and puts in into N (INT) variable.
 */
void read_size();

/*
 * @Arg: char grid[N][N]
 * @Return type: void
 * @Returns: nothing
 * Fills the grid with '-' after reading the grid size
 */
void initialize_grid(char grid[N][N]);

/*
 * @Arg: char grid[N][N]
 * @Return type: void
 * @Returns: nothing
 * Prints grid values in square-shaped pattern
 */
void print_grid(char grid[N][N]);

/*
 * @Arg: char grid[N][N], int *player
 * @Return type: void
 * @Returns: nothing
 * Asks user to enter next move coordinates (Y,X), here Y,X>0
 * Checks for invalid coordinates, and returns an warning to user on non-valid input and returning
 * If valid, places a mark
 */
void make_move(char grid[][N], int *player);

/*
 * @Arg: char grid[N][N]
 * @Return type: INT
 * @Returns: 1, if Player 1 won, 2, if Player 2 won, 0, if neither of them won
 *
 * Checks for vertical wins
 * Checks for horizontal wins
 * Checks for diagonal wins
 */
int row_covered(char grid[N][N], int player);

/*
 * @Arg: char grid[N][N]
 * @Return type: _BOOL (alias bool)
 * @Returns: true if the grid is full, false if the grid is not full
 *
 * Checks whether the game ended in stalemate or not
 */
bool stalemate(char grid[N][N]);


int main()
{
    printf("WELCOME!\n\n");

    read_size(); //reading grid size before declaring an array (as memory for an array needs to be declared beforehand)

    char grid[N][N]; //two-dimensional array used to store the grid

    initialize_grid(grid); //fill the array with '-'

    int player = 1; //player identifier (1 - Player 1, 2 - Player 2)

    //infinite loop for constantly asking players to place marks unless game finished in win or stalemate
    while(true)
    {
        putchar('\n');
        make_move(grid, &player);
        print_grid(grid);
        putchar('\n');

        if(row_covered(grid, player) > 0) {
            printf("\n<<<<<<<Player has %d won!<<<<<<<\n", row_covered(grid, player));
            break;
        }

        if(stalemate(grid)){
            printf("\n<<<<<<<Game ended in Stalemate!<<<<<<<\n");
            break;
        }
    }

    printf("\n-------Goodbye!-------\n"); //exit message

    return 0;
}

void read_size()
{
    printf("Enter board size: ");
    scanf("%d", &N);
}


void initialize_grid(char grid[N][N])
{
    //placing '-' symbol everywhere on the board (for display purposes)
    for(int y = 0; y<N; y++)
        for(int x = 0; x<N; x++)
            grid[y][x] = '-';

    //printing the initial grid to user (players)
    for(int y = 0; y<N; y++)
    {
        for(int x = 0; x<N; x++)
        {
            if(x == 0) printf("- ");
            else printf(" - ");
        }
        printf("\n");
    }
}


void print_grid(char grid[N][N])
{
    for(int y = 0; y<N; y++) {
        for (int x = 0; x < N; x++) {
            if (x == 0) printf("%c", grid[y][x]);
            else printf(" %c ", grid[y][x]);
        }
        printf("\n");
    }
}

void make_move(char grid[][N], int *player)
{

    //Reading mark coordinates from player
    int Xcoord = 0;
    int Ycoord = 0;

    printf("Player %d - Place mark: ", *player);
    scanf("%d, %d", &Ycoord, &Xcoord);

    /* Checking if entered coordinates are valid
     * Validation criteria:
     * 1. Coordinates have bounds: X,Y >= 0 AND X,Y <= N
     * 2. Entered coordinates cannot be reserved (mark cannot be equal to 'X' or 'O')
     */
    if((Xcoord > N || Ycoord > N) || Xcoord <= 0 || Ycoord <= 0 || grid[Ycoord-1][Xcoord-1] == 'X' || grid[Ycoord-1][Xcoord-1] == 'O')
    {
        printf("Invalid mark! Please try again!\n"); //display warning message to player
        return; //return (this lets user to try again)
    }

    /*
     * HUMAN - MACHINE COORDINATE TRANSLATION
     * Since user can enter coordinates from 0 to N, and array bounds are from 0 to (N-1),
     * the code below adapts coordinates that player is familiar with to coordinates that a program is familiar with
     */
    Xcoord--;
    Ycoord--;

    for(int y = 0; y<N; y++)
    {
        for(int x = 0; x<N; x++)
        {
            if(x == Xcoord && y == Ycoord) //check if coordinates math (invalid coordinates checked above)
            {
                /*
                 * Switch-case statement
                 * Purpose: checks with player is playing and places a mark accordingly
                 * Exception: in case the player variable goes beyond bounds (1 or 2).
                 *            Displays a warning to user and automatically sets current player to 1
                 */
                switch (*player)
                {
                    case 1:
                        grid[y][x] = 'X';
                        *player = 2;
                        break;
                    case 2:
                        grid[y][x] = 'O';
                        *player = 1;
                        break;
                    default:
                        printf("Critical Error: invalid player detected.\n");
                        printf("Due to a critical error, it's now player's 1 move.\n");
                        *player = 1;
                        break;
                }
            }
        }
    }
}

int row_covered(char grid[N][N], int player)
{
    //checking for horizontal wins
    for(int y = 0; y<N; y++)
    {
        int Xcount = 0, Ocount = 0;
        for(int x = 0; x<N; x++)
        {
            if(grid[y][x] == 'X') Xcount++;
            else if(grid[y][x] == 'O') Ocount++;
        }

        if(Xcount == N) return 1;
        else if(Ocount == N) return 2;
    }

    //checking for vertical wins
    for(int x = 0; x<N; x++)
    {
        int Xcount = 0, Ocount = 0;
        for(int y = 0; y<N; y++)
        {
            if(grid[y][x] == 'X') Xcount++;
            else if(grid[y][x] == 'O') Ocount++;
        }

        if(Xcount == N) return 1;
        else if(Ocount == N) return 2;
    }

    //checking for diagonal wins (from top left to bottom right)
    int Xcount = 0, Ocount = 0;
    for(int i = 0; i<N; i++)
    {
        if(grid[i][i] == 'X') Xcount++;
        else if(grid[i][i] == 'O') Ocount++;
    }
    if(Xcount == N) return 1;
    else if(Ocount == N) return 2;

    // //checking for diagonal wins (from top right to bottom left)
    // Xcount = 0;
    // Ocount = 0;

    // for(int i = 0; i<N; i++)
    // {
    //     if(grid[i][N-i-1] == 'X') Xcount++;
    //     else if(grid[i][N-i-1] == 'O') Ocount++;
    // }
    // if(Xcount == N) return 1;
    // else if(Ocount == N) return 2;



    //Marlene's mind (DELETE)
    int win = 1;
    for(int i = 0; i<N; i++)
    {
        if(!(grid[N-i-1][i] == grid[N-i-2][i+1]) || grid[N-i-1][i] == '-' || grid[N-i-2][i+1] == '-')
        {
            win = 0;
        }
    }
    if(win) return player;
    //if nobody won, return 0
    return 0;


}

/*
 * Algorithm:
 * 1. Since this function is called after if statement that checks for wins
 * 2. If nobody has won, there are 2 options: either there are empty marks or no empty marks and no win
 * 3. In case of second situation, there is a stalemate. This could be checked whether all marks are occupied by 'X'
 * and 'O'
 */
bool stalemate(char grid[N][N])
{
    int full_count = 0;

    for(int y = 0; y<N; y++)
        for(int x = 0; x<N; x++)
            if(grid[y][x] == 'X' || grid[y][x] == 'O') full_count++;

    if(full_count == N*N) return true;
    return false;
}