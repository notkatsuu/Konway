// Optimized game.c
#include "game.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------
// Create / Destroy
// ---------------------------------------------------------
Grid *create_grid(int rows, int cols) {
    Grid *g = (Grid *)malloc(sizeof(Grid));
    g->rows = rows;
    g->cols = cols;
    g->cells = calloc(rows * cols, sizeof(int)); // calloc initializes memory to zero
    return g;
}

void destroy_grid(Grid *grid) {
    if (!grid) return;
    free(grid->cells);
    free(grid);
}

// ---------------------------------------------------------
// Helpers
// ---------------------------------------------------------
void clear_grid(Grid *grid) {
    memset(grid->cells, 0, grid->rows * grid->cols * sizeof(int));
}

void randomize_grid(Grid *grid) {
    for (int i = 0; i < grid->rows * grid->cols; i++) {
        grid->cells[i] = rand() % 2;
    }
}

static inline int idx(const Grid *g, int row, int col) {
    return row * g->cols + col;
}

int count_neighbors(const Grid *grid, int x, int y) {
    static const int offsets[] = {-1, 0, 1};
    int count = 0;
    for (int dx = 0; dx < 3; dx++) {
        for (int dy = 0; dy < 3; dy++) {
            if (dx == 1 && dy == 1) continue; // Skip the current cell
            int nx = (x + offsets[dx] + grid->rows) % grid->rows;
            int ny = (y + offsets[dy] + grid->cols) % grid->cols;
            count += grid->cells[idx(grid, nx, ny)];
        }
    }
    return count;
}

void next_generation(const Grid *current, Grid *next) {
    int rows = current->rows, cols = current->cols;
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            int neighbors = count_neighbors(current, x, y);
            next->cells[idx(next, x, y)] = (current->cells[idx(current, x, y)] && (neighbors == 2 || neighbors == 3)) ||
                                          (!current->cells[idx(current, x, y)] && neighbors == 3);
        }
    }
}

bool grids_are_equal(const Grid *g1, const Grid *g2) {
    return memcmp(g1->cells, g2->cells, g1->rows * g1->cols * sizeof(int)) == 0;
}

void copy_grid(const Grid *src, Grid *dst) {
    memcpy(dst->cells, src->cells, src->rows * src->cols * sizeof(int));
}

// ---------------------------------------------------------
// Pattern Detection
// ---------------------------------------------------------
uint64_t hash_grid(const Grid *g) {
    uint64_t hash = 1469598103934665603ULL;
    const uint64_t fnvPrime = 1099511628211ULL;
    for (int i = 0; i < g->rows * g->cols; i++) {
        hash ^= (uint64_t)g->cells[i];
        hash *= fnvPrime;
    }
    return hash;
}

int detect_pattern(const Grid * const prevStates[], const uint64_t prevHashes[],
                   const Grid *current, uint64_t curHash, int generation, Options options) {
    if (generation == 0) return -1;

    if (curHash == prevHashes[generation - 1] &&
        grids_are_equal(current, prevStates[generation - 1])) {
        return 0; // Static pattern
        }

    if (options.stopOnLooping) {
        for (int gen = 0; gen < generation - 1; gen++) {
            if (curHash == prevHashes[gen] &&
                grids_are_equal(current, prevStates[gen])) {
                return 1; // Looping pattern
                }
        }
    }

    if (options.stopOnGliding && detect_gliders(current)) {
        return 2; // Glider detected
    }

    return -1;
}

bool detect_gliders(const Grid *grid) {
    static const int gliderPatterns[4][3][3] = {
        {{0, 1, 0}, {0, 0, 1}, {1, 1, 1}},
        {{1, 0, 1}, {1, 0, 0}, {1, 1, 0}},
        {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}},
        {{0, 1, 1}, {1, 1, 0}, {0, 0, 1}}};

    for (int x = 1; x < grid->rows - 3; x++) {
        for (int y = 1; y < grid->cols - 3; y++) {
            for (int p = 0; p < 4; p++) {
                bool match = true;
                for (int i = -1; i <= 3; i++) {
                    for (int j = -1; j <= 3; j++) {
                        if (i >= 0 && i < 3 && j >= 0 && j < 3) {
                            // Check pattern cells
                            if (grid->cells[idx(grid, x + i, y + j)] != gliderPatterns[p][i][j]) {
                                match = false;
                                break;
                            }
                        } else {
                            // Check surrounding border cells
                            if (grid->cells[idx(grid, x + i, y + j)] != 0) {
                                match = false;
                                break;
                            }
                        }
                    }
                    if (!match) break;
                }
                if (match) return true;
            }
        }
    }
    return false;
}
