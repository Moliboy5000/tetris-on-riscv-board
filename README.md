
# Tetris on the dtek-board
## Rocco Molero and Vilmer Palmér - IS1500 Computer Organization and Components
### Objective and requirements
The goal of this project was to implement the classic game of tetris, playable on the dtek-board.
 The idea being to have the dtek-board connected to a vga screen displaying the game graphics,
  while controlling the game with the buttons and levers on the dtek-board. The main requirements for the program were defined as follows:
* The program should implement the basic game logic of the tetris game, with different types of blocks falling down a grid, and full rows on the grid clearing with the rows above dropping down in response. The game should terminate if it is not possible to place a piece upon spawning.
* The game graphics should be visible on a vga monitor connected to the dtek-board changing responsively according to the game logic.
* The game controls, such as moving and rotating a piece, should be implemented on the buttons and levers on the dtek-board (see Solution for specifics).
* The game should have a scoreboard, shown on the 7-segment display on the dtek-board.
* The game speed, or the speed at which the blocks drop, should gradually increase based on the time elapsed.

### Solution
The first step of the development process involved creating the basic game logic of the tetris game,
so that it could be verified independently that the game logic itself has been correctly implemented.
We coded a tetris game playable from the terminal using the ncurses library,
where the pieces are represented as differently colored brackets ([]).
The position of the different blocks is represented by a 20x10 grid,
which is used by the various logic components of the game as well as for displaying the game graphics.
In this implementation, the pieces are moved and rotated by keyboard controls and the timing is controlled by a clock using the time library. The idea was that with a base implementation of the game, it would be fairly simple and straightforward to switch the graphical aspects to the vga screen, the controls to the dtek-board and the clock to the one on the dtek-board.

After creating the basic structure of the game, it was then a matter of translating this to be visible on the screen and compatible with the dtek-board.
We made functions that translate a grid representing the state of the game (colors and positions of blocks) to the pixels on the screen buffer.
The playing grid is centered on the screen, with a white outline on the sides to separate it from the rest of the screen.

Implementing the controls was a particular issue, since we do not make use of a joystick used in most traditional tetris game controls. We ended up with the following control scheme:
* Pressing the button with all switches off rotates the current piece counter-clockwise.
* Pressing the button with the left-most switch on moves the current piece one step to the left.
* Pressing the button with the right-most switch on moves the current piece one step to the right.

The scoreboard was implemented on the seven segment display, which shows the lines cleared. After each line clear,
the timeouts per tick are decreased to make the pieces fall faster. Timeouts are detected via interrupts.
The button status and switch status is checked directly in the main function. Each button press also disables the button actions for a short amount of time in order to avoid rapid actions from one button press. The program checks the buttons for input, and uses timer interrupts to control the time. Switch status is checked directly, not using interrupts.

### Verification
Verification of the program essentially involved checking that all the requirements defined earlier were met. If the game was playable without any bugs or issues, it works! We played through the game several times, each time testing that different aspects of the game, such as lines clearing and the fail condition triggering, were correctly implemented.

### Contributions
All design choices were decided upon together, and in general most of the development was done collaboratively rather than individually. However, we did decide to divide certain aspects of the project to improve effectiveness:

* Rocco was tasked with implementing the base logic of the tetris game and making a preliminary version playable from the terminal.
* Vilmer was responsible for implementing proper communication with the vga screen, and making the game graphics visible on it.

Integrating the tetris game with the dtek-board, translating the controls and implementing the clock with timer interrupts was done collaboratively, as well as the writing of this document. Additionally, we used previous code from lab 3 in the course. See the source code for explicit details regarding contributions.

### Reflections
We believe that this project has served as a great learning experience, and we are satisfied with our results. Working on such a low level of programming certainly comes with its own aspects that are rarely considered in high level programming, but it makes even the simpler feats feel much more rewarding. Regardless of whether or not we end up working on a hardware level in the future, having at least some familiarity in the area is most definitely useful for understanding how a computer works on a fundamental level.

