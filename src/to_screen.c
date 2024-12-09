// This file was written by Vilmer Palmér

#include <stdint.h>
#include "globals.h"

#define SCREENBUFFER 0x08000000
#define SCREEN_W 320
#define SCREEN_H 240
#define OFFSET 100

#define RED 0xE0
#define CYAN 0x1F
#define BLUE 0x3
#define ORANGE 0xF0
#define YELLOW 0xFC
#define GREEN 0x1C
#define PURPLE 0xE3
#define WHITE 0xFF

int colorList[] = {
    0,
    RED,
    CYAN,
    BLUE,
    ORANGE,
    YELLOW,
    GREEN,
    PURPLE,
    WHITE};

volatile uint8_t *screenbuffer = (uint8_t *)SCREENBUFFER;

// draws one square
void draw_square(int x, int y, int colorIndex)
{
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            screenbuffer[(y + i) * SCREEN_W + (OFFSET + x + j)] = colorList[colorIndex];
        }
    }
}

// updates the screen according to the grid
void to_screen()
{
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            draw_square(x * 12, y * 12, grid[y][x]);
        }
    }
}

// resets all squares to black
void reset_screen()
{
    for (int i = 0; i < SCREEN_W * SCREEN_H; i++)
    {
        screenbuffer[i] = 0;
    }
}

// draws an outline around the grid
void draw_outline()
{
    for (int y = 0; y < SCREEN_H; y++)
    {
        screenbuffer[y * SCREEN_W + 99] = WHITE;
        screenbuffer[y * SCREEN_W + 98] = WHITE;
        screenbuffer[y * SCREEN_W + 220] = WHITE;
        screenbuffer[y * SCREEN_W + 221] = WHITE;
    }
}

// resets screen and draws the outline
void setup_screen()
{
    reset_screen();
    draw_outline();
}