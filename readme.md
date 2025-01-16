# Conway's Game of Life

A graphical implementation of Conway's Game of Life using the Raylib library. This project showcases dynamic grid-based simulations with user interaction and pattern detection. The program allows for customization of grid size, speed, and detection settings, offering a fun and educational experience.

## Features
- **Graphical Interface**: Powered by Raylib, the program provides a user-friendly visual environment.
- **Dynamic Grid Size**: Set grid dimensions interactively or via command-line arguments.
- **Pattern Detection**: Detect static patterns, loops, and gliders in the grid.
- **Interactive Editor**: Draw and randomize grid patterns before running the simulation.
- **Adjustable Speed**: Control the simulation speed during runtime.

## Installation
### Dependencies
This project requires X11 for GUI rendering on Linux systems. To install the necessary dependency, run:
```bash
sudo apt-get install libx11-dev
```
For more information on Raylib dependencies, visit the [Raylib Linux guide](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

### Building the Project
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-folder>
   ```
2. Build the program using the provided Makefile:
   ```bash
   make
   ```
3. The executable will be located in the `project` folder.

### Running the Program
To run the program:
```bash
./project/conway [rows] [cols]
```
- **Without Arguments**: The program will prompt you to set the grid resolution interactively.
- **With Arguments**: Specify the grid dimensions directly (e.g., `./project/conway 20 20`).

## Usage
### Grid Resolution Setup
- Adjust rows/columns with arrow keys.
- Hold `ALT` for a square grid.
- Hold `SHIFT` for larger increments (x10).

### Editor Mode
- Randomize grid: Press `R`
- Clear grid: Press `C`
- Toggle cells: Click with the mouse
- Start simulation: Press `ENTER`
- Enable/disable pattern detection:
    - `1`: Toggle glider detection
    - `2`: Toggle loop/static detection

### Simulation Mode
- Pause/Resume: Press `SPACE`
- Adjust speed: Use arrow keys
- Return to editor: Press `R`
- Exit program: Press `ESC`

## Program Workflow
1. **Splash Screen**: Displays a welcoming splash screen with project details.
2. **Grid Resolution Setup**: Interactive or argument-based setup of grid dimensions.
3. **Editor Mode**: Design grid patterns or randomize them for testing.
4. **Simulation**: Runs the Game of Life simulation, detects patterns, and handles user input dynamically.
5. **Detection**: Stops the simulation when patterns are detected (static, looping, or glider).

## Known Issues
- **Multi-Monitor Setup**: On WSL2, the program may not recognize the primary display correctly.
- **Grid Scaling**: Non-square grid sizes might cause minor visual offsets at the edges.
- **Increment Limitation**: When one grid dimension exceeds 90, scaling x10 is disabled for the other dimension.

## Screenshots

### Grid Resolution Setup
_Set the grid dimensions interactively or via command-line arguments._

![Grid Resolution Setup Placeholder](C:\Users\pauca\Documents\GitHub\Conway\readme\Picture5.png)

### Splash Screen
_A beautifully designed splash screen welcomes you to the program._

![Splash Screen Placeholder](C:\Users\pauca\Documents\GitHub\Conway\readme\Picture1.png)

### Editor Mode
_Design and randomize grid patterns with full control._

![Editor Mode Placeholder](C:\Users\pauca\Documents\GitHub\Conway\readme\Picture2.png)

### Simulation Mode
_Run the simulation, detect patterns, and adjust speed._

![Simulation Mode Placeholder](C:\Users\pauca\Documents\GitHub\Conway\readme\Picture3.png)

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

---
Enjoy exploring Conway's Game of Life with this interactive and graphical implementation!

