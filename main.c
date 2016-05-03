#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

static int terminal_size(unsigned *width, unsigned *height) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0)
        return -1;
    *width = w.ws_col;
    *height = w.ws_row;
    return 0;
}

static void print_board(bool *board, unsigned width, unsigned height) {
    /* Move to (0, 0). */
    printf("\033[1;1H");

    /* Print the board. */
    for (unsigned i = 0; i < height; i++) {
        for (unsigned j = 0; j < width; j++)
            putchar(board[i * width + j] ? 178 : ' ');
        putchar('\n');
    }
}

static void turn(bool *board, bool *new, unsigned width, unsigned height) {
    for (unsigned i = 0; i < height; i++) {
        for (unsigned j = 0; j < width; j++) {

            /* Find how many live neighbours this cell has. Note that we only use a finite board. */
            unsigned neighbours = 0;
            if (i > 0 && j > 0)                  neighbours += board[(i - 1) * width + j - 1];
            if (i > 0)                           neighbours += board[(i - 1) * width + j];
            if (i > 0 && j < width - 1)          neighbours += board[(i - 1) * width + j + 1];
            if (j > 0)                           neighbours += board[i * width + j - 1];
            if (j < width - 1)                   neighbours += board[i * width + j + 1];
            if (i < height - 1 && j > 0)         neighbours += board[(i + 1) * width + j - 1];
            if (i < height - 1)                  neighbours += board[(i + 1) * width + j];
            if (i < height - 1 && j < width - 1) neighbours += board[(i + 1) * width + j + 1];

            /* Now the rules of the game itself. */
            if (board[i * width + j]) {
                switch (neighbours) {
                    case 0 ... 1:
                    case 4 ... 8:
                        new[i * width + j] = false;
                        break;
                    case 2 ... 3:
                        new[i * width + j] = true;
                        break;
                }
            } else if (neighbours == 3) {
                new[i * width + j] = true;
            } else {
                new[i * width + j] = false;
            }
        }
    }
}

int main(void) {
    unsigned width;
    unsigned height;

    if (terminal_size(&width, &height) != 0) {
        fprintf(stderr, "failed to get terminal dimensions\n");
        return EXIT_FAILURE;
    }

    bool *b1 = calloc(width * height, sizeof(bool));
    bool *b2 = calloc(width * height, sizeof(bool));
    if (b1 == NULL || b2 == NULL) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    /* Initialise the board. */
    srand(time(NULL));
    for (unsigned i = 0; i < width * height; i++)
        b1[i] = rand() % 2;
    bool *board = b1;

    /* Repeatedly mutate the board. */
    for (;;) {
        print_board(board, width, height);
        if (board == b1) {
            turn(b1, b2, width, height);
            board = b2;
        } else {
            turn(b2, b1, width, height);
            board = b1;
        }
        sleep(1);
    }

    return EXIT_SUCCESS;
}
