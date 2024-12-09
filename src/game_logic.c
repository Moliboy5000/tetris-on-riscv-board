// This file was mostly written by Rocco Molero

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "globals.h"
#include "tetromino.h"

#define WIDTH 10
#define HEIGHT 20

int grid[HEIGHT][WIDTH] = {0};

// all pieces
const Tetromino tetrominoes[7] = {
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, 1}, // Z
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, 2}, // I
    {{{0, 0}, {0, 1}, {1, 1}, {2, 1}}, 3}, // J
    {{{0, 1}, {1, 1}, {2, 1}, {2, 0}}, 4}, // L
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}, 5}, // O
    {{{0, 1}, {1, 1}, {1, 0}, {2, 0}}, 6}, // S
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}, 7}  // T
};

Tetromino currentPiece;
int currentX = 4, currentY = 0;
int linesCleared = 0;

// Place a piece on the grid
void placePiece()
{
    for (int i = 0; i < 4; i++)
    {
        int nx = currentX + currentPiece.blocks[i].x;
        int ny = currentY + currentPiece.blocks[i].y;
        grid[ny][nx] = currentPiece.color;
    }
}

// remove a peice from the grid
void removePiece()
{
    for (int i = 0; i < 4; i++)
    {
        int nx = currentX + currentPiece.blocks[i].x;
        int ny = currentY + currentPiece.blocks[i].y;
        grid[ny][nx] = 0; // 0 = black
    }
}

// turn a piece white
void oldPiece()
{
    for (int i = 0; i < 4; i++)
    {
        int nx = currentX + currentPiece.blocks[i].x;
        int ny = currentY + currentPiece.blocks[i].y;
        grid[ny][nx] = 8;
    }
}

// Check if a piece can be placed
// NOTE: We worked together on this
bool canPlace(Tetromino piece, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        int nx = x + piece.blocks[i].x;
        int ny = y + piece.blocks[i].y;
        if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT)
        {
            return false;
        }
        if (grid[ny][nx] == 8) // index > 7 means not current piece
        {
            return false;
        }
    }
    return true;
}

// Remove full lines and adjust level
int removeLines()
{
    int cleared = 0;
    for (int y = HEIGHT - 1; y >= 0; y--)
    {
        bool full = true;
        for (int x = 0; x < WIDTH; x++)
        {
            if (!grid[y][x])
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int ny = y; ny > 0; ny--)
            {
                for (int x = 0; x < WIDTH; x++)
                {
                    grid[ny][x] = grid[ny - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++)
            {
                grid[0][x] = 0;
            }
            y++; // Check the same row again
            cleared++;
        }
    }
    return cleared;
}

// Rotate the current piece
Tetromino
rotatePiece(Tetromino piece)
{
    Tetromino rotated = piece;
    for (int i = 0; i < 4; i++)
    {
        int tmp = rotated.blocks[i].x;
        rotated.blocks[i].x = -rotated.blocks[i].y;
        rotated.blocks[i].y = tmp;
    }
    return rotated;
}

// reset grid
void resetGrid()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            grid[i][j] = 0;
        }
    }
}

// spawn a random(kinda) piece, two of the same piece will not spawn in a consecutively
void spawnPiece(int timeoutcount)
{
    Tetromino newPiece = tetrominoes[timeoutcount % 7];
    if (currentPiece.color == newPiece.color)
    {
        currentPiece = tetrominoes[timeoutcount % 7 + 1];
    }
    else
    {
        currentPiece = newPiece;
    }
    currentX = 4;
    currentY = 0;
}
