#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "game.h"
#include "ui.h"

typedef enum {
    STATE_MENU,
    STATE_SIMULATION
} AppState;

typedef struct {
    AppState state;
    Grid *current;
    Grid *next;
    Grid *historyStates[MAX_HISTORY];
    uint64_t historyHashes[MAX_HISTORY];
    Options options;
    bool paused;
    bool running;
    int generation;
    float simulationSpeed;
    float accumulator;
    float stepTime;
    int cellSize;
} GameState;

void handle_menu(GameState *gameState);
void handle_simulation(GameState *gameState);
void draw_menu(const GameState *gameState);
void draw_simulation(const GameState *gameState);

int main(int argc, char *argv[]) {
    int rows = 10, cols = 10;
    if (argc == 3) { // Check if there were args of rows and cols then use them
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
    }
    //if there weren't args, then ask for input with selection screen
    if (!select_resolution_if_needed(&rows, &cols, argc == 3)) return 0;

    //Remove window header and resizing
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
    InitWindow(1200, 600, "Conway's Game of Life");
    SetTargetFPS(60);

    show_splash_screen();

    GameState gameState = {
        .state = STATE_MENU,
        .current = create_grid(rows, cols),
        .next = create_grid(rows, cols),
        .options = { .stopOnGliding = false, .stopOnLooping = true },
        .paused = false,
        .running = true,
        .generation = 0,
        .simulationSpeed = 1.0f,
        .accumulator = 0.0f,
        .stepTime = 0.05f,
        .cellSize = calculate_cell_size(rows, cols)
    };

    for (int i = 0; i < MAX_HISTORY; i++) { //Fill historyStates with empty grids
        gameState.historyStates[i] = create_grid(rows, cols);
        gameState.historyHashes[i] = 0;
    }
    clear_grid(gameState.current); //Clear the grid

    while (!WindowShouldClose()) {
        if (gameState.state == STATE_MENU) {
            handle_menu(&gameState);
        } else if (gameState.state == STATE_SIMULATION) {
            handle_simulation(&gameState);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (gameState.state == STATE_MENU) {
            draw_menu(&gameState);
        } else if (gameState.state == STATE_SIMULATION) {
            draw_simulation(&gameState);
        }
        draw_signature();
        EndDrawing();
    }

    destroy_grid(gameState.current);
    destroy_grid(gameState.next);
    for (int i = 0; i < MAX_HISTORY; i++) {
        destroy_grid(gameState.historyStates[i]);
    }
    CloseWindow();
    return 0;
}

void handle_menu(GameState *gameState) {
    if (handle_menu_input(gameState->current, &gameState->options, &gameState->cellSize)) {
        gameState->state = STATE_SIMULATION;
        gameState->generation = 0;
        gameState->paused = false;
        gameState->running = true;
    }
}

void handle_simulation(GameState *gameState) {
    if (IsKeyPressed(KEY_SPACE)) gameState->paused = !gameState->paused;
    if (IsKeyPressed(KEY_UP)) gameState->simulationSpeed = fminf(gameState->simulationSpeed * 2.0f, 8.0f);
    if (IsKeyPressed(KEY_DOWN)) gameState->simulationSpeed = fmaxf(gameState->simulationSpeed / 2.0f, 0.125f);
    if (IsKeyPressed(KEY_R)) {
        clear_grid(gameState->current);
        gameState->generation = 0;
        gameState->state = STATE_MENU;
        gameState->paused = false;
        gameState->running = true;
        return;
    }

    if (!gameState->paused && gameState->running) {
        float dt = GetFrameTime();
        gameState->accumulator += dt * gameState->simulationSpeed;
        while (gameState->accumulator >= gameState->stepTime) {
            gameState->accumulator -= gameState->stepTime;

            if (gameState->generation < MAX_HISTORY) {
                copy_grid(gameState->current, gameState->historyStates[gameState->generation]);
                gameState->historyHashes[gameState->generation] = hash_grid(gameState->current);
            }

            if (gameState->generation > 0 && gameState->generation < MAX_HISTORY) {
                int detection = detect_pattern(
                    (const Grid * const *)gameState->historyStates,
                    gameState->historyHashes,
                    gameState->current,
                    hash_grid(gameState->current),
                    gameState->generation,
                    gameState->options
                );
                if (detection != -1) {
                    gameState->running = false;
                    gameState->paused = true;
                    break;
                }
            }


            next_generation(gameState->current, gameState->next);
            Grid *temp = gameState->current;
            gameState->current = gameState->next;
            gameState->next = temp;
            gameState->generation++;
        }
    }
}

void draw_menu(const GameState *gameState) {
    draw_menu_ui(gameState->current, gameState->options, gameState->cellSize);
}

void draw_simulation(const GameState *gameState) {
    int detection = -1;
    if (gameState->generation > 0 && gameState->generation < MAX_HISTORY) {
        detection = detect_pattern(
            (const Grid * const *)gameState->historyStates, //Cast to const pointer necessary for the compiler ?¿
            gameState->historyHashes,
            gameState->current,
            hash_grid(gameState->current),
            gameState->generation,
            gameState->options
        );
    }
    draw_simulation_ui(
        gameState->current,
        gameState->generation,
        gameState->paused,
        gameState->running,
        gameState->simulationSpeed,
        detection,
        gameState->cellSize
    );
}
