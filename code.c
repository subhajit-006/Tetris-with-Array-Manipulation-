
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // For _kbhit and _getch in Windows

#define WIDTH 10
#define HEIGHT 20

int board[HEIGHT][WIDTH] = {0};

// Tetromino shapes
int tetromino[7][4][4] = {
    // I
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},
    // O
    {{0,0,0,0},
     {0,1,1,0},
     {0,1,1,0},
     {0,0,0,0}},
    // T
    {{0,0,0,0},
     {1,1,1,0},
     {0,1,0,0},
     {0,0,0,0}},
    // S
    {{0,0,0,0},
     {0,1,1,0},
     {1,1,0,0},
     {0,0,0,0}},
    // Z
    {{0,0,0,0},
     {1,1,0,0},
     {0,1,1,0},
     {0,0,0,0}},
    // J
    {{0,0,0,0},
     {1,0,0,0},
     {1,1,1,0},
     {0,0,0,0}},
    // L
    {{0,0,0,0},
     {0,0,1,0},
     {1,1,1,0},
     {0,0,0,0}}
};

int current[4][4];
int x = 3, y = 0;
int shape;

void drawBoard() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j]) printf("#");
            else printf(".");
        }
        printf("\n");
    }
}

void copyShape() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            current[i][j] = tetromino[shape][i][j];
}

int checkCollision(int nx, int ny) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if (current[i][j]) {
                int newX = nx + j;
                int newY = ny + i;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX]))
                    return 1;
            }
        }
    return 0;
}

void mergeShape() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (current[i][j] && y+i >= 0)
                board[y+i][x+j] = 1;
}

void clearLines() {
    for (int i = HEIGHT-1; i >= 0; i--) {
        int full = 1;
        for (int j = 0; j < WIDTH; j++)
            if (!board[i][j]) full = 0;
        if (full) {
            for (int k = i; k > 0; k--)
                for (int j = 0; j < WIDTH; j++)
                    board[k][j] = board[k-1][j];
            for (int j = 0; j < WIDTH; j++)
                board[0][j] = 0;
            i++;
        }
    }
}

void newShape() {
    shape = rand() % 7;
    copyShape();
    x = 3;
    y = 0;
    if (checkCollision(x, y)) {
        printf("Game Over!\n");
        exit(0);
    }
}

void rotate() {
    int tmp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[j][3-i] = current[i][j];

    int old[4][4];
    memcpy(old, current, sizeof(old));
    memcpy(current, tmp, sizeof(current));
    if (checkCollision(x, y))
        memcpy(current, old, sizeof(current));
}

int main() {
    srand(time(0));
    newShape();
    while (1) {
        drawBoard();

        if (_kbhit()) {
            char c = _getch();
            if (c == 'a' && !checkCollision(x-1, y)) x--;
            else if (c == 'd' && !checkCollision(x+1, y)) x++;
            else if (c == 's' && !checkCollision(x, y+1)) y++;
            else if (c == 'w') rotate();
        }

        if (!checkCollision(x, y+1)) {
            y++;
        } else {
            mergeShape();
            clearLines();
            newShape();
        }

        for (volatile int i = 0; i < 10000000; i++);
    }
    return 0;
}
