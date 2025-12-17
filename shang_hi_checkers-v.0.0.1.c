/*

 A program so two players can have a game of checkers.

*/

// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Macros
//
#define ROWS 9
#define COLS 9
#define MAX_INPUT 4

#define COLOR_BOLD              "\x1b[1m"

#define ANSI_COLOR_BLACK        "\x1b[30m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_BG_COLOR_RED       "\x1b[41m"
#define ANSI_BG_COLOR_GREEN     "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW    "\x1b[43m"
#define ANSI_BG_COLOR_BLUE      "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA   "\x1b[45m"
#define ANSI_BG_COLOR_CYAN      "\x1b[46m"
#define ANSI_COLOR_RESET        "\x1b[0m"

// Structs
//
struct checkersboard {
    char ckboard[ROWS][COLS];
};

struct player_move {
    int y;
    int x;
};

// Globals
//
char emptyboard[ROWS][COLS] = {
        ' ', '1', '2',  '3',  '4',  '5',  '6',  '7',  '8',
        '1', '_', 'X',  '_',  'X',  '_',  'X',  '_',  'X',
        '2', 'X', '_',  'X',  '_',  'X',  '_',  'X',  '_',
        '3', '_', 'X',  '_',  'X',  '_',  'X',  '_',  'X',
        '4', 'X', '_',  'X',  '_',  'X',  '_',  'X',  '_',
        '5', '_', 'X',  '_',  'X',  '_',  'X',  '_',  'X',
        '6', 'X', '_',  'X',  '_',  'X',  '_',  'X',  '_',
        '7', '_', 'X',  '_',  'X',  '_',  'X',  '_',  'X',
        '8', 'X', '_',  'X',  '_',  'X',  '_',  'X',  '_',
    };

// Function Declarations
//
void make_a_move_1(struct checkersboard* gameboard, int r, int c);
void make_a_move_2(struct checkersboard* gameboard, int r, int c);
void got_jumped(struct checkersboard* gameboard);
void new_queen(struct checkersboard* gameboard);
void initialize_gameboard(struct checkersboard* gameboard);
void print_board(struct checkersboard* gameboard);
void save_gameboard(struct checkersboard* gameboard);
struct checkersboard* load_gameboard(struct checkersboard* gameboard);
int trap();


/*
 * Main Function
 *
*/
int main() {

    int selection;
    int t = trap();
    int r = t / 9;
    int c = t % 9;

    setbuf(stdout, NULL); // necessary for immediate respose as nc server

    puts(ANSI_COLOR_GREEN \
        "======= Getting Out the Board ======="
        ANSI_COLOR_RESET "\n");

    struct checkersboard* gameboard = malloc(sizeof(struct checkersboard));
    memcpy(gameboard, emptyboard, sizeof(struct checkersboard));

    print_board(gameboard);

    puts("");
    printf(ANSI_COLOR_RED \
          "Beware! Trap is located @ row:%d column:%d\n" \
          ANSI_COLOR_RESET, r, c);
    puts("");

    initialize_gameboard(gameboard);
menu:
    print_board(gameboard);
    puts("\n\tMake a selection:");
    puts(ANSI_COLOR_BLUE \
        "\t1. Player 1 (O) Moves" \
        ANSI_COLOR_RESET);
    puts(ANSI_COLOR_MAGENTA \
        "\t2. Player 2 (@) Moves" \
        ANSI_COLOR_RESET);
    puts(ANSI_COLOR_RED \
        "\t3. Remove Jumped Piece"
        ANSI_COLOR_RESET);
    puts(ANSI_COLOR_YELLOW \
        "\t4. Exit" \
        ANSI_COLOR_RESET);
    puts(ANSI_COLOR_GREEN \
        "\t5. Get A Queen" \
        ANSI_COLOR_RESET);
    puts(ANSI_COLOR_CYAN \
        "\t6. Save Game" \
        ANSI_COLOR_RESET);
    puts("\t7. Load Game");
    scanf("%d", &selection);
    getchar();

    while (selection != 4) {
        if (selection == 1) {
            make_a_move_1(gameboard, r, c);
        }
        else if (selection == 2) {
            make_a_move_2(gameboard, r, c);
        }
        else if (selection == 3) {
            got_jumped(gameboard);
        }
        else if (selection == 5) {
            new_queen(gameboard);
        }
        else if (selection == 6) {
            save_gameboard(gameboard);
        }
        else if (selection == 7) {
            load_gameboard(gameboard);
        }
        else {
            puts("****** Please make a valid selection ******\n");
        }
        goto menu;
    }

    puts(ANSI_COLOR_BLUE \
        "\n\t    DID YOU TIE?\n" \
        ANSI_COLOR_RESET);

    return EXIT_SUCCESS;
}

/*
 * Function to mark the new position on the board
 * and unmark the old position for player 1.
 *
 * Arg:     gameboard   struct pointer to the board object
 * Arg:     r           the row position on the board
 * Arg:     c           the column position on the board
 *
 * Returns the updated checkersboard object
 *
*/
void make_a_move_1(struct checkersboard* gameboard, int r, int c) {

    // Player 1 makes a move
    struct player_move start;
    struct player_move fin;

    puts("Player 1, ready to make a move?");
    puts("Enter ROW number for starting move:");
    scanf("%d", &start.y);
    puts("Enter COLUMN number for starting move:");
    scanf("%d", &start.x);

    puts("Enter ROW number for destination:");
    scanf("%d", &fin.y);
    puts("Enter COLUMN number for destination:");
    scanf("%d", &fin.x);

    if (start.y > 8 || start.x > 8
        || fin.y > 8 || fin.x > 8) {
        puts("\nCoordinates provided is not a space on the board.");
        puts("--------------- Try Again! ---------------\n");
    }
    else if (gameboard->ckboard[start.y][start.x] == '_' ||
             gameboard->ckboard[start.y][start.x] == 'X' ||
             gameboard->ckboard[start.y][start.x] == '@' ) {
        puts("\nStarting coordinates is empty space or opponents piece.");
        puts("--------------- Try Again! ---------------\n");
    }
    else if (gameboard->ckboard[fin.y][fin.x] != '_') {
        puts("\nDestination coordinates is not a valid space.");
        puts("--------------- Try Again! ---------------\n");
    }

    if (gameboard->ckboard[start.y][start.x] == 'Q')
        gameboard->ckboard[fin.y][fin.x] = 'Q';
    else
        gameboard->ckboard[fin.y][fin.x] = 'O';

    if (fin.y == r && fin.x == c) {
       gameboard->ckboard[fin.y][fin.x] = '_'; 
    }

    gameboard->ckboard[start.y][start.x] = '_';

    puts("");

}

// Function to mark the new position on the board and unmark the old pos for p2.
void make_a_move_2(struct checkersboard* gameboard, int r, int c) {

    // Player 2 makes a move
    struct player_move start;
    struct player_move fin;

    puts("Player 2, ready to make a move?");
    puts("Enter ROW number for starting move:");
    scanf("%d", &start.y);
    puts("Enter COLUMN number for starting move:");
    scanf("%d", &start.x);

    puts("Enter ROW number for destination:");
    scanf("%d", &fin.y);
    puts("Enter COLUMN number for destination:");
    scanf("%d", &fin.x);

    if (start.y > 8 || start.x > 8
        || fin.y > 8 || fin.x > 8) {
        puts("\nCoordinates provided is not a space on the board.");
        puts("--------------- Try Again! ---------------\n");
    }
    else if (gameboard->ckboard[start.y][start.x] == '_' ||
             gameboard->ckboard[start.y][start.x] == 'X' ||
             gameboard->ckboard[start.y][start.x] == 'O' ) {
        puts("\nStarting coordinates is empty space or opponents piece.");
        puts("--------------- Try Again! ---------------\n");
    }
    else if (gameboard->ckboard[fin.y][fin.x] != '_') {
        puts("\nDestination coordinates is not a valid space.");
        puts("--------------- Try Again! ---------------\n");
        return;
    }

    if (gameboard->ckboard[start.y][start.x] == 'Q')
        gameboard->ckboard[fin.y][fin.x] = 'Q';
    else
        gameboard->ckboard[fin.y][fin.x] = '@';

    if (fin.y == r && fin.x == c) {
       gameboard->ckboard[fin.y][fin.x] = '_'; 
    }

    gameboard->ckboard[start.y][start.x] = '_';

    puts("");

}

// Function to remove jumped pieces from the board
void got_jumped(struct checkersboard* gameboard) {

    int a, b;
    char check[MAX_INPUT];
    check[MAX_INPUT] = 0;

    puts(ANSI_COLOR_RED \
        "Did you jump your opponets piece? y/n" \
        ANSI_COLOR_RESET);

    fgets(check, MAX_INPUT, stdin);

    if (check[0] == 'y') {

        puts("Enter ROW number for the jumped piece:");
        scanf("%d", &a);
        puts("Enter COLUMN number for the jumped piece:");
        scanf("%d", &b);

        if (gameboard->ckboard[a][b] == 'X') {
            puts("***** No piece at those coordinates! *****");
            puts("--------------- Try Again! ---------------\n");
        }
        else
            gameboard->ckboard[a][b] = '_';
    }
    else {
        puts("Returning to menu...\n");
    }

}

// Function to put a new queen on the board
void new_queen(struct checkersboard* gameboard) {

    int a, b;
    char check[MAX_INPUT];
    check[MAX_INPUT] = 0;

    puts(ANSI_COLOR_RED \
        "Did the piece reach the opponet's back row? y/n" \
        ANSI_COLOR_RESET);

    fgets(check, MAX_INPUT, stdin);

    if (check[0] == 'y') {
        struct player_move queen;
        puts("Enter ROW number for the new queen:");
        scanf("%d", &a);
        puts("Enter COLUMN number for the new queen:");
        scanf("%d", &b);
        if (gameboard->ckboard[a][b] == 'X') {
            puts(ANSI_COLOR_RED \
                "******* Incorrect coordinates *******" \
                ANSI_COLOR_RESET);
            puts("--------------- Try Again! ---------------");
        }
        else 
            gameboard->ckboard[a][b] = 'Q';
    }
    else {
        puts("Returning to menu...\n");
    }

}

// Function to introduce a trap to make things fun!
int trap() {

    puts("");
    puts(ANSI_COLOR_RED \
         "  <<<<<<<<<<  歡迎 光臨 >>>>>>>>>> ");
    puts("  <<<<<<<<<<   SHANGHI  >>>>>>>>>> ");
    puts("  <<<<<<<<<<  CHECKERS! >>>>>>>>>> " \
        ANSI_COLOR_RESET);
    puts("");

    int n = 81;
    srand(time(0));
    int value = rand() % (n + 1);
    if (value % 2 == 0)

        return value;

    return value + 1;
}

// Function to set the starting position of the pieces on the board
void initialize_gameboard(struct checkersboard* gameboard) {
    puts("+++++++ Starting A New Game! +++++++");

    gameboard->ckboard[1][1] = 'O';
    gameboard->ckboard[1][3] = 'O';
    gameboard->ckboard[1][5] = 'O';
    gameboard->ckboard[1][7] = 'O';
    gameboard->ckboard[2][2] = 'O';
    gameboard->ckboard[2][4] = 'O';
    gameboard->ckboard[2][6] = 'O';
    gameboard->ckboard[2][8] = 'O';
    gameboard->ckboard[3][1] = 'O';
    gameboard->ckboard[3][3] = 'O';
    gameboard->ckboard[3][5] = 'O';
    gameboard->ckboard[3][7] = 'O';

    gameboard->ckboard[6][2] = '@';
    gameboard->ckboard[6][4] = '@';
    gameboard->ckboard[6][6] = '@';
    gameboard->ckboard[6][8] = '@';
    gameboard->ckboard[7][1] = '@';
    gameboard->ckboard[7][3] = '@';
    gameboard->ckboard[7][5] = '@';
    gameboard->ckboard[7][7] = '@';
    gameboard->ckboard[8][2] = '@';
    gameboard->ckboard[8][4] = '@';
    gameboard->ckboard[8][6] = '@';
    gameboard->ckboard[8][8] = '@';

    // trap location
    //gameboard->ckboard[r][c] = '_';
    puts("");
}

// Function that prints the gameboard
void print_board(struct checkersboard* gameboard) {
    for (int i = 0; i < ROWS; i++) {
        printf("\t ");
        for (int j = 0; j < COLS; j++) {
            printf("%s" "%s" "%c " "%s", \
                  ANSI_COLOR_BLACK, ANSI_BG_COLOR_YELLOW, \
                  gameboard->ckboard[i][j], \
                  ANSI_COLOR_RESET);
        }
        printf("\t\n");
    }
}

// Function to save the configuration of the gameboard
void save_gameboard(struct checkersboard* gameboard) {
    FILE* save = fopen("checkers+.bin", "wb");
    fwrite(gameboard, 81, 1, save);
    puts(ANSI_COLOR_YELLOW \
        "^^^^^^^ Game Saved ^^^^^^^\n"
        ANSI_COLOR_RESET);
}

// Function to load a saved gameboard configuration
struct checkersboard* load_gameboard(struct checkersboard* gameboard) {
    FILE* load = fopen("checkers+.bin", "rb");
    fread(gameboard, 81, 1, load);
    puts(ANSI_COLOR_GREEN \
        "\n$$$$$$$$$ Yo! Game Board Loaded $$$$$$$$$\n" \
        ANSI_COLOR_RESET);
    return gameboard;
}
