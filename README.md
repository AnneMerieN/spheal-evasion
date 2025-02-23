# Spheal Evasion

**Spheal Evasion** is a simple 2D game built in C using [Raylib](https://www.raylib.com/). In this game, you control a small Spheal that collects fish while evading a chasing orca. The game features a welcoming menu screen, collectible items, and a game-over state that allows you to restart without having to close the program.

## Features

- **Home/Welcome Screen:**  
  A blue-themed menu that welcomes you to Spheal Adventure. Press **ENTER** to start the game.

- **Gameplay Mechanics:**  
  - **Movement:** Use the **W**, **A**, **S**, and **D** keys to move Spheal in all four directions.
  - **Collectibles:** A fish spawns at a random location. When Spheal touches the fish, it is collected and disappears, and your fish counter increases.
  - **Enemy Behavior:** An orca chases Spheal. If the orca collides with Spheal, the game ends.

- **Game Over & Restart:**  
  When Spheal collides with the orca, a "Game Over" screen is displayed. Press **R** to return to the menu and restart the game.

## Controls

- **W / A / S / D:** Move Spheal up, left, down, and right.
- **ENTER:** Start the game from the welcome screen.
- **R:** Restart the game when on the Game Over screen.

## Requirements

- **Raylib:**  
  Download and install Raylib from [raylib.com](https://www.raylib.com/).

- **C Compiler:**  
  Use a C compiler such as GCC to build the game.

## Compilation

For Linux or macOS, you can compile the game with a command similar to:

```bash
gcc spheal_adventure.c -o spheal_adventure -lraylib -lm -lpthread -ldl -lrt -lX11
