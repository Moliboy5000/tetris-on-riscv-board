
// main.c

// This file was originally written 2024 by Artur Podobas and Pedro Antunes
// for lab3 dtek. It was as recommended used as a base for the mini project by
// Vilmer Palmér and Rocco Molero 2024.
// this file was mostly written by Vilmer Palmér

#include <stdio.h>
#include <stdbool.h>
#include "tetromino.h"
#include "globals.h"

#include <stdbool.h>

extern void print(const char *);
extern void print_dec(unsigned int);
extern void delay(int);
extern void enable_interrupt();

extern void to_screen();

extern void setup_screen();
extern bool canPlace();
extern int removeLines();
extern void placePiece();
extern void removePiece();
extern void spawnPiece();
extern void resetGrid();
extern void oldPiece();

volatile int *display = (volatile int *)0x04000050;
volatile int *switches = (volatile int *)0x04000010;
volatile int *button = (volatile int *)0x040000d0;
volatile int *timer = (volatile int *)0x04000020;

int timer_period = 300000;
int timeoutcount = 0;
int display_converter[] = {0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0011000};

int scoreArr[] = {0, 0, 0, 0, 0, 0};

// sets a display
void set_displays(int display_number, int value)
{
  volatile int *address;
  address = (volatile int *)0x04000050 + (4 * display_number);
  *address = 0x00;
  *address = *address | display_converter[value];
}

// returns switch status
int get_sw(void)
{
  return *switches;
}

// returns 1 when button is pressed, 0 when not
int get_button(void)
{
  return *button & 1;
}

// restarts the timer
void reset_timer()
{
  *timer &= 0b1111111111111110;      // sets timeout to 0
  *(timer + 4) = *(timer + 2);       // snapl = periodl
  *(timer + 5) = *(timer + 3);       // snaph = periodh
  *(timer + 1) |= 0b000000000000100; // set START bit to 1
  timeoutcount++;
}

// initializes interrupts
void init(void)
{
  *(timer + 3) = (timer_period & 0b11111111111111110000000000000000) >> 16; // periodl
  *(timer + 2) = timer_period & 0b00000000000000001111111111111111;         // periodh
  *(timer + 1) |= 0b000000000000100;                                        // set START bit to 1
  *(timer + 1) |= 0b000000000000001;                                        // set ITO to 1 (enable interrupts)
  // *(switches + 2) |= 0b0000000100;
  enable_interrupt();
}

// updates the 7-figure display
void display_score()
{
  for (int i = 0; i <= 5; i++)
  {
    set_displays(i, scoreArr[i]);
  }
}

// updates the score array
void update_score(int score)
{
  for (int i = 0; i < 6; i++)
  {
    scoreArr[i] = score % 10;
    score /= 10;
  }
}

// checks if timeout
int is_timeout()
{
  if ((*timer & 0b000000000000001) == 1)
  {
    return 1;
  }
  return 0;
}

int timeouts_per_tick = 60;
bool buttonEnabled = true;

// handles interrupts. Will regularly drop, spawn or place
// the current piece. Also removes lines and updates score.
// NOTE: We worked together on most of this
void handle_interrupt(unsigned cause)
{
  if (cause == 16) // timer
  {
    if (timeoutcount % timeouts_per_tick == 0)
    {
      {
        // drop every tick
        if (canPlace(currentPiece, currentX, currentY + 1))
        {
          removePiece();
          currentY++;
          placePiece();
        }
        else // if cant drop more
        {
          placePiece();
          oldPiece();
          int newCleared = removeLines();
          if (newCleared > 0)
          {
            if (timeouts_per_tick > 20)
            {
              timeouts_per_tick -= 10;
            }
            linesCleared += newCleared;
            update_score(linesCleared);
            display_score();
          }
          spawnPiece(timeoutcount);
          if (!canPlace(currentPiece, currentX, currentY))
          {
            resetGrid();
            linesCleared = 0;
          }
          else
          {
            placePiece();
          }
        }
        to_screen();
      }
    }
    reset_timer();
  }
}

// runs
int main()
{
  // initializes stuff
  init();
  reset_timer();
  setup_screen();
  spawnPiece();
  placePiece();
  to_screen(grid);
  int counter = 0;

  while (1)
  {
    counter++;
    if (get_button() == 1 && buttonEnabled)
    {
      buttonEnabled = false;
      {
        int switches = get_sw();
        if (switches & 0b0000000001)
        {
          if (canPlace(currentPiece, currentX + 1, currentY))
          {
            removePiece();
            currentX++;
            placePiece();
          }
        }
        else if (switches & 0b1000000000)
        {
          if (canPlace(currentPiece, currentX - 1, currentY))
          {
            removePiece();
            currentX--;
            placePiece();
          }
        }
        else
        {
          Tetromino rotated = rotatePiece(currentPiece);
          if (canPlace(rotated, currentX, currentY))
          {
            removePiece();
            currentPiece = rotated;
            placePiece();
          }
          to_screen();
        }
      }
    }

    if (counter % 500000 == 0) // ugly but button seems to spam rising edges
    {
      buttonEnabled = true;
    }
  }