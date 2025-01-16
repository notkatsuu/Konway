#include "ui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "game.h"

// Define color scheme
static Color backgroundColor = BLACK;
static Color gridColor       = (Color){ 255, 255, 255, 20 };
static Color aliveCellColor  = (Color){ 200, 255, 255, 200 };
static Color deadCellColor   = (Color){ 255, 255, 255, 10 };
static Color textColor       = LIGHTGRAY;

#define MIN_CELL_SIZE 1
#define MAX_CELL_SIZE 100

static inline int idx(const Grid *g, int row, int col) {
    return row * g->cols + col;
}


void show_splash_screen(void) {
    bool showSplashScreen = true;
    float splashScreenTimer = 5.0f;
    double startTime = GetTime();

    while (showSplashScreen && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw your splash screen text
        DrawText("Laboratory 1: Conway's Game of Life", 100, 100, 50, RAYWHITE);
        DrawText("Authors: Pau Castellà, Lluc Koome", 100, 180, 30, LIGHTGRAY);
        DrawText("Course: Operating Systems (2024-2025)", 100, 250, 30, LIGHTGRAY);
        DrawText("Press SPACE to continue", 100, 450, 30, SKYBLUE);
        DrawText("This window will close automatically in", 100, 500, 30, DARKGRAY);
        DrawText(TextFormat("%.0f seconds", splashScreenTimer - (GetTime() - startTime)), 680, 500, 30, GRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_SPACE) || (GetTime() - startTime >= splashScreenTimer)) {
            showSplashScreen = false;
        }
    }
}

bool select_resolution_if_needed(int *rows, int *cols, bool resolutionProvided) {
    if (resolutionProvided) return true;  // Already have rows/cols from args

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
    InitWindow(600, 300, "Set Grid Resolution");
    SetTargetFPS(60);

    bool startSimulation = false;

    const int MIN_ROWS = 10;
    const int MIN_COLS = 10;
    const int MAX_ROWS = 100;
    const int MAX_COLS = 100;

    while (!WindowShouldClose() && !startSimulation) {
        bool altPressed = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
        bool shiftPressed = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        int step = (shiftPressed && *rows <= 90 && *cols <= 90) ? 10 : 1;

        if ((IsKeyPressedRepeat(KEY_UP) || IsKeyPressed(KEY_UP)) && *rows < MAX_ROWS) {
            *rows += step;
            if (altPressed) *cols = *rows; // Keep square aspect ratio
        }
        if ((IsKeyPressedRepeat(KEY_DOWN) || IsKeyPressed(KEY_DOWN)) && *rows > MIN_ROWS) {
            *rows -= step;
            if (altPressed) *cols = *rows; // Keep square aspect ratio
        }
        if ((IsKeyPressedRepeat(KEY_RIGHT) || IsKeyPressed(KEY_RIGHT)) && *cols < MAX_COLS) {
            *cols += step;
            if (altPressed) *rows = *cols; // Keep square aspect ratio
        }
        if ((IsKeyPressedRepeat(KEY_LEFT) || IsKeyPressed(KEY_LEFT)) && *cols > MIN_COLS) {
            *cols -= step;
            if (altPressed) *rows = *cols; // Keep square aspect ratio
        }

        if (IsKeyPressed(KEY_ENTER)) {
            startSimulation = true;
        }

        BeginDrawing();
        ClearBackground(backgroundColor);

        DrawText("Adjust grid resolution:", 110, 30, 30, textColor);
        DrawText("Rows (Up/Down):", 110, 80, 20, textColor);
        DrawText("Columns (Left/Right):", 110, 120, 20, textColor);


        if (altPressed) {
            DrawText("[ALT] Square Aspect Ratio Enabled", 110, 160, 20, GREEN);
        } else {
            DrawText("[ALT] Square Aspect Ratio Disabled", 110, 160, 20, RED);
        }
        if (shiftPressed) {
            DrawText("[SHIFT] Scaling 10x Faster", 110, 200, 20, GREEN);
        } else {
            DrawText("[SHIFT] Scaling 1x Faster", 110, 200, 20, RED);
        }


        DrawText(TextFormat("%d", *rows), 400, 75, 30, SKYBLUE);
        DrawText(TextFormat("%d", *cols), 400, 115, 30, SKYBLUE);

        DrawText("Press ENTER to start simulation", 110, 240, 20, SKYBLUE);

        EndDrawing();
    }

    CloseWindow();
    return startSimulation;
}


int calculate_cell_size(int rows, int cols) {
    // Example: base the size on minimum of window width/cols and window height/rows
    int ySize = GetScreenHeight() / rows;
    int xSize = (GetScreenWidth() / 1.8f) / cols;

    int cellSize = fminf(xSize, ySize);

    if (cellSize < MIN_CELL_SIZE) cellSize = MIN_CELL_SIZE;
    if (cellSize > MAX_CELL_SIZE) cellSize = MAX_CELL_SIZE;

    return cellSize;
}

// Draw grid
static void draw_grid(const Grid *grid, int cellSize) {
    // Calculate the maximum width and height for the grid to fit within screenWidth / 2
    int maxWidth = GetScreenWidth() / 2;
    int maxHeight = GetScreenHeight();

    // Recalculate cellSize if necessary to ensure the grid fits within the left half
    int maxCellWidth = maxWidth / grid->cols;
    int maxCellHeight = maxHeight / grid->rows;
    int adjustedCellSize = fminf(cellSize, fminf(maxCellWidth, maxCellHeight));

    // Calculate the total width and height of the grid
    int gridWidth = grid->cols * adjustedCellSize;
    int gridHeight = grid->rows * adjustedCellSize;

    // Calculate the offsets to center the grid within the left half of the screen
    int offsetX = (maxWidth - gridWidth) / 2;
    int offsetY = (maxHeight - gridHeight) / 2;

    // Darken the left part of the screen
    DrawRectangle(0, 0, maxWidth, maxHeight, (Color){30, 30, 30, 150});

    // Draw the grid
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            int index = idx(grid, i, j);
            bool isAlive = (grid->cells[index] != 0);

            // Calculate the position of each cell with the offsets
            int x = offsetX + j * adjustedCellSize;
            int y = offsetY + i * adjustedCellSize;

            if (isAlive) {
                DrawRectangle(x, y, adjustedCellSize, adjustedCellSize, aliveCellColor);
            } else {
                DrawRectangle(x, y, adjustedCellSize, adjustedCellSize, deadCellColor);
                DrawRectangleLines(x, y, adjustedCellSize, adjustedCellSize, gridColor);
            }
        }
    }
}

bool handle_restart(Grid **current, Grid **next, Grid **historyStates, int *rows, int *cols, int *cellSize) {
    if (IsKeyPressed(KEY_B)) {
        // Restart logic
        if (!select_resolution_if_needed(rows, cols, false)) {
            CloseWindow();
            exit(0);  // Exit if resolution selection is canceled
        }

        // Free current grids
        destroy_grid(*current);
        destroy_grid(*next);
        for (int i = 0; i < MAX_HISTORY; i++) {
            destroy_grid(historyStates[i]);
        }

        // Recreate grids with new dimensions
        *current = create_grid(*rows, *cols);
        *next = create_grid(*rows, *cols);
        for (int i = 0; i < MAX_HISTORY; i++) {
            historyStates[i] = create_grid(*rows, *cols);
        }

        // Recalculate cell size
        *cellSize = calculate_cell_size(*rows, *cols);

        return true;  // Indicate that a restart occurred
    }

    return false;  // No restart occurred
}

bool handle_menu_input(Grid *grid, Options *options, int *cellSize) {
    static int lastPaintedX = -1;
    static int lastPaintedY = -1;
    bool startSimulation = false;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        // Adjust for grid offset and cell size
        int offsetX = (GetScreenWidth() / 2 - (grid->cols * *cellSize)) / 2;
        int offsetY = (GetScreenHeight() - (grid->rows * *cellSize)) / 2;

        int x = (mouse.y - offsetY) / *cellSize;
        int y = (mouse.x - offsetX) / *cellSize;

        if (x >= 0 && x < grid->rows && y >= 0 && y < grid->cols &&
            (x != lastPaintedX || y != lastPaintedY)) {
            int index = idx(grid, x, y);
            // Toggle cell state
            grid->cells[index] = !grid->cells[index];
            lastPaintedX = x;
            lastPaintedY = y;
            }
    } else {
        lastPaintedX = -1;
        lastPaintedY = -1;
    }


    // Randomize
    if (IsKeyPressed(KEY_R)) {
        randomize_grid(grid);
    }

    // Clear
    if (IsKeyPressed(KEY_C)) {
        clear_grid(grid);
    }

    // Start simulation
    if (IsKeyPressed(KEY_ENTER)) {
        startSimulation = true;
    }

    // Toggle options
    if (IsKeyPressed(KEY_ONE)) options->stopOnGliding = !options->stopOnGliding;
    if (IsKeyPressed(KEY_TWO)) options->stopOnLooping = !options->stopOnLooping;



    return startSimulation;
}




void draw_menu_ui(const Grid *grid, Options options, int cellSize) {
    // Draw the grid within the left half of the screen
    draw_grid(grid, cellSize);

    // Draw the text within the right half of the screen
    int textStartX = GetScreenWidth() / 2 + 40;

    // Controls Title
    DrawText("Controls:", textStartX, 50, 26, WHITE);

    // [R]: Randomize Grid
    DrawText("[", textStartX, 100, 20, textColor);
    DrawText("R", textStartX + 10, 100, 20, SKYBLUE);
    DrawText("]: Randomize Grid", textStartX + 27, 100, 20, textColor);

    // [C]: Clear Grid
    DrawText("[", textStartX, 130, 20, textColor);
    DrawText("C", textStartX + 10, 130, 20, SKYBLUE);
    DrawText("]: Clear Grid", textStartX + 27, 130, 20, textColor);

    // [ENTER]: Start Simulation
    DrawText("[", textStartX, 160, 20, textColor);
    DrawText("ENTER", textStartX + 10, 160, 20, SKYBLUE);
    DrawText("]: Start Simulation", textStartX + 85, 160, 20, textColor);

    // [Mouse]: Paint Cells
    DrawText("[", textStartX, 190, 20, textColor);
    DrawText("LEFT CLICK", textStartX + 10, 190, 20, SKYBLUE);
    DrawText("(inside a cell)]: Paint Cells", textStartX + 140, 190, 20, textColor);


    // Options Title
    DrawText("Options:", textStartX, 400, 26, WHITE);

    // [1]: Stop on Gliding
    DrawText("[", textStartX, 440, 20, textColor);
    DrawText("1", textStartX + 10, 440, 20, SKYBLUE);
    DrawText("]: Stop on Gliding: ", textStartX + 22, 440, 20, textColor);
    DrawText(options.stopOnGliding ? "ON" : "OFF", textStartX + 250, 440, 20,
             options.stopOnGliding ? GREEN : RED);

    // [2]: Stop on Looping
    DrawText("[", textStartX, 480, 20, textColor);
    DrawText("2", textStartX + 10, 480, 20, SKYBLUE);
    DrawText("]: Stop on Looping: ", textStartX + 27, 480, 20, textColor);
    DrawText(options.stopOnLooping ? "ON" : "OFF", textStartX + 250, 480, 20,
             options.stopOnLooping ? GREEN : RED);
}


void draw_simulation_ui(const Grid *grid, int generation, bool paused, bool running,
                     float simulationSpeed, int detection, int cellSize)
{
    draw_grid(grid, cellSize);

    // Info text
    DrawText(TextFormat("Generation: %d", generation), GetScreenWidth()/2 + 40, 30, 30, RAYWHITE);
    DrawText(paused ? "PAUSED (Press SPACE to Resume)" : "RUNNING (Press SPACE to Pause)",
             GetScreenWidth()/2 + 40, 100, 20, textColor);

    if (simulationSpeed == 8.0f) {
        DrawText("Speed: REALTIME (Use UP/DOWN to adjust)", GetScreenWidth()/2 + 40, 160, 20, textColor);
    } else {
        DrawText(TextFormat("Speed: x%.2f (Use UP/DOWN to adjust)", simulationSpeed),
                 GetScreenWidth()/2 + 40, 160, 20, textColor);
    }

    if (!running) {
        char message[100];
        if (detection == 0) {
            sprintf(message, "SIMULATION STOPPED (Static Pattern Detected)");
        } else if (detection == 1) {
            sprintf(message, "SIMULATION STOPPED (Looping Detected)");
        } else if (detection == 2) {
            sprintf(message, "SIMULATION STOPPED (Glider Detected)");
        } else {
            sprintf(message, "SIMULATION STOPPED");
        }
        DrawText(message, GetScreenWidth()/2 + 40, 220, 20, RED);
        DrawText("Press R to restart simulation or ESC to exit",
                 GetScreenWidth()/2 + 40, 280, 20, GREEN);
    }
}

void draw_signature(void) {
    const char *text = "by Pau \"Katsu\" Castellà and Lluc Koome";
    int textSize = 20;
    int textWidth = MeasureText(text, textSize);
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    DrawText(text, screenW - textWidth - 10, screenH - textSize - 10, textSize, DARKGRAY);
}
