#include <stdio.h>

#define MAZE_WIDTH  20
#define MAZE_HEIGHT 10

char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
    "####################",
    "#P . . . # . . . . #",// Pac-Man starts at position (1, 1)
    "# #### # # ### ####",
    "# #    # # #   #   #",
    "# # #### # # # # # #",
    "# # #    . . . # # #",
    "# # #### ##### # # #",
    "# #      #     #   #",
    "# ########## ### ###",
    "####################"
};

int pacman_x; //Column
int pacman_y; //Row

void initialize_game() {
    // Find Pac-man's starting position
    for (int i =0; i <MAZE_WIDTH; i++) {
        for (int j =0; j <MAZE_WIDTH; j++) {
            if (maze[i][j] == 'P')
                {
                    pacman_y = i;
                    pacman_x = j;
                    maze[i][j] = ' ';//Once found, replace 'P' with an empty space in the maze
                                     // This is so we can redraw 'P' at its new position later
                    return; // Pac-man found, no need to search further                 //
          }
        }
    }
}

void draw_maze() {
    for (int i = 0; i < MAZE_HEIGHT; i++){
        for (int j = 0; j < MAZE_HEIGHT; j++){
            printf("%c", maze[i][j]);
        }
        printf("\n");    }
}

int main() {
    printf("Welcome to C-Maze!\n");

    initialize_game();
    char original_tile = maze[pacman_y][pacman_x]; // Stores what was there before Pac-Man
    maze[pacman_y][pacman_x] = 'P'; // places Pac-Man at its starting location for the first draw

    draw_maze(); // Draws the maze with Pac-Man
    maze[pacman_y][pacman_x] = original_tile;


    printf("pac-man's starting position: (%d, %d)\n", pacman_y, pacman_x);

    return 0;
}
