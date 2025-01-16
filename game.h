#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>   // <--- IMPORTANT: for uint64_t

#define MAX_HISTORY 10000

// Options for pattern detection
typedef struct {
 bool stopOnGliding;
 bool stopOnLooping;
} Options;

// Flattened grid structure
typedef struct {
 int rows;
 int cols;
 int *cells;  // Single, contiguous array of size rows*cols
} Grid;


/**
 * Create a new grid with the given dimensions (rows x cols).
 * Allocates memory for the Grid struct and for the 2D array of cells.
 */
Grid *create_grid(int rows, int cols);
void destroy_grid(Grid *grid);



/**
 * Create a history (an array) of Grid pointers for up to maxHistory generations.
 */
Grid **create_history(int maxHistory, int rows, int cols);

/**
 * Destroy the history array of Grid pointers.
 */
void destroy_history(Grid **history, int maxHistory);

/**
 * Clear the grid (set all cells to 0).
 */
void clear_grid(Grid *grid);


/**
 * Randomize the grid (set cells randomly to 0 or 1).
 */
void randomize_grid(Grid *grid);

/**
 * Copy the source grid's cells into the destination grid.
 */
void copy_grid(const Grid *src, Grid *dst);

/**
 * Count the number of alive neighbors (with wrapping).
 */
int count_neighbors(const Grid *grid, int x, int y);

/**
 * Compute the next generation of cells from current -> next.
 */
void next_generation(const Grid *current, Grid *next);

/**
 * Check if two grids have identical cell data.
 */
bool grids_are_equal(const Grid *g1, const Grid *g2);

/**
 * Detect patterns:
 *  - Returns  0 if the grid is the same as the immediate previous generation (static).
 *  - Returns  1 if the grid matches *any* earlier generation (looping).
 *  - Returns  2 if a glider is detected (only if stopOnGliding = true).
 *  - Returns -1 if no pattern found.
 */
int detect_pattern(const Grid * const prevStates[], const uint64_t prevHashes[],
                   const Grid *current, uint64_t curHash, int generation, Options options);



/**
 * Detect if a glider is present in the grid.
 */
bool detect_gliders(const Grid *grid);

/**
 * Helper that checks if there is a glider in a 3x3 block at position (x, y).
 */
bool is_glider(const Grid *grid, int x, int y);

// Hash-based approach
uint64_t hash_grid(const Grid *g);




#endif // GAME_H
