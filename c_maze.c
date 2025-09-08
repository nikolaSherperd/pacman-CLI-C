#include <stdio.h>
#include <termios.h> // For terminal raw mode (more advanced input)
#include <unistd.h>  // For STDIN_FILENO

#define MAZE_WIDTH  20
#define MAZE_HEIGHT 10

char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
    "####################",
    "#P . . . # . . . . #",
    "# #### # # ### ####",
    "# #    # # #   #   #",
    "# # #### # # # # # #",
    "# # #    . . . # # #",
    "# # #### ##### # # #",
    "# #      #     #   #",
    "# ########## ### ###",
    "####################"
};

int pacman_x;
int pacman_y;

// Keep track of collected dots (simple game goal)
int score = 0;
int total_dots = 0; // We'll count these at the start

// --- Function Prototypes ---
// Declare functions before main so compiler knows they exist
void initialize_game();
void draw_maze();
void setup_terminal_input(); // For non-blocking/raw input
void restore_terminal_input(); // To reset terminal mode
// --------------------------

void initialize_game() {
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            if (maze[i][j] == 'P') {
                pacman_y = i;
                pacman_x = j;
                maze[i][j] = ' '; // Replace 'P' with empty space in the map
            }
            if (maze[i][j] == '.') {
                total_dots++; // Count total dots for score tracking
            }
        }
    }
}

// Function to clear screen and draw maze
void draw_maze() {
    // ANSI escape codes:
    // \033[H   - Move cursor to home position (top-left)
    // \033[J   - Clear screen from cursor to end
    printf("\033[H\033[J");

    // Temporarily place Pac-Man for drawing
    char original_tile_at_pacman = maze[pacman_y][pacman_x];
    maze[pacman_y][pacman_x] = 'P';

    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    // Restore the tile for game logic
    maze[pacman_y][pacman_x] = original_tile_at_pacman;

    printf("Score: %d / %d\n", score, total_dots); // Display score
    printf("Use W A S D to move. Press Q to quit.\n");
}


// --- Terminal Input Handling (Advanced but necessary for smooth game) ---
// We need to save the original terminal settings to restore them later
static struct termios old_tio, new_tio;

void setup_terminal_input() {
    tcgetattr(STDIN_FILENO, &old_tio); // Get current terminal settings
    new_tio = old_tio; // Copy them

    // Set new terminal settings:
    // ICANON - Disable canonical mode (don't wait for Enter)
    // ECHO   - Disable echoing characters back to the terminal
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio); // Apply settings immediately
}

void restore_terminal_input() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // Restore original settings
}
// --------------------------------------------------------------------------


int main() {
    initialize_game();
    setup_terminal_input(); // Set terminal to raw mode

    char input;
    int next_x, next_y; // Where Pac-Man WANTS to move

    // Game Loop
    while (1) { // Loop indefinitely until 'Q' is pressed
        draw_maze(); // Redraw the entire maze

        // Read single character input without waiting for Enter
        input = getchar();

        next_x = pacman_x;
        next_y = pacman_y;

        switch (input) {
            case 'w': // Move Up
                next_y--;
                break;
            case 's': // Move Down
                next_y++;
                break;
            case 'a': // Move Left
                next_x--;
                break;
            case 'd': // Move Right
                next_x++;
                break;
            case 'q': // Quit Game
                goto end_game; // A jump statement, used here to break out of nested loops/functions cleanly
            default:
                // If invalid input, don't move
                continue;
        }

        // --- Collision Detection and Movement ---
        // Check if the next position is within maze boundaries
        if (next_x >= 0 && next_x < MAZE_WIDTH &&
            next_y >= 0 && next_y < MAZE_HEIGHT) {

            // Check if the next position is a wall
            if (maze[next_y][next_x] != '#') {
                // Not a wall, so it's a valid move!

                // Check if we collected a dot
                if (maze[next_y][next_x] == '.') {
                    score++;
                    maze[next_y][next_x] = ' '; // Eat the dot!
                }

                // Update Pac-Man's actual position
                pacman_x = next_x;
                pacman_y = next_y;
            }
        }
        // If Pac-Man has collected all dots, game over!
        if (score == total_dots) {
            draw_maze(); // Draw one last time with all dots eaten
            printf("Congratulations! You ate all the dots!\n");
            goto end_game;
        }
    }

end_game: // Label for goto statement
    restore_terminal_input(); // Restore original terminal settings
    printf("Thanks for playing C-Maze!\n");
    return 0;
}
