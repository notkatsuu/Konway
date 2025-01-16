#ifndef UI_H
#define UI_H

#include "game.h"

/**
 * Show a splash screen (lab details, authors, etc.) for a few seconds or until user presses SPACE.
 */
void show_splash_screen(void);

/**
 * Show a resolution selection window if the user didn’t provide rows/cols as arguments.
 * Updates `rows` and `cols` with the user’s choices.
 * Returns true if user confirmed to start, false if user closed the window.
 */
bool select_resolution_if_needed(int *rows, int *cols, bool resolutionProvided);

/**
 * Draw the menu (the place where user can paint cells, randomize, etc.).
 */
void draw_menu_ui(const Grid *grid, Options options, int cellSize);

/**
 * Handle user input in the menu (like toggling cells with mouse, randomize, clear, etc.).
 * Returns true if user wants to start the simulation, false otherwise.
 */
bool handle_menu_input(Grid *grid, Options *options, int *cellSize);

/**
 * Draw the simulation interface: draw the cells, generation info, paused/running state, etc.
 */
void draw_simulation_ui(const Grid *grid, int generation, bool paused, bool running, float simulationSpeed, int detection, int cellSize);

/**
 * Adjust the cellSize based on the current window height.
 */
int calculate_cell_size(int rows, int cols);

/**
 * Display a small signature text at bottom-right of the screen.
 */
void draw_signature(void);


#endif // UI_H
