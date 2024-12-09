// This file was written by Rocco Molero

#ifndef TETROMINO_H
#define TETROMINO_H

typedef struct
{
    int x, y;
} Point;

typedef struct
{
    Point blocks[4];
    int color;
} Tetromino;

extern Tetromino currentPiece;
Tetromino rotatePiece(Tetromino piece);

#endif