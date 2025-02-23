#include "raylib.h"
#include <math.h>

// Define the game states.
typedef enum GameScreen { MENU = 0, GAMEPLAY, GAMEOVER } GameScreen;

int main(void) {
    // Window dimensions
    const int screenWidth = 800;
    const int screenHeight = 460;
    InitWindow(screenWidth, screenHeight, "Spheal Evasion!");

    // Load textures (ensure these files are in your working directory)
    Texture2D sphealTexture = LoadTexture("Spheal.png");
    Texture2D orcaTexture = LoadTexture("orca-whale.png");
    Texture2D fishTexture   = LoadTexture("fish.png");

    // Scale factors for each image
    float sphealScale = 0.1f; // spheal image is small
    float orcaScale   = 0.5f; // orca image is larger
    float fishScale   = 0.3f; // adjust as needed for fish size

    // Game state variable
    GameScreen currentScreen = MENU;

    // Declare game variables (they will be reset when starting gameplay)
    int sphealX = 200, sphealY = 200;
    int orcaX = 400, orcaY = 0;
    int fishX = 0, fishY = 0;
    bool fishActive = false;
    int fishCollected = 0;
    float chaseSpeed = 3.0f;

    // Set FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update logic based on the current game screen.
        switch (currentScreen) {
            case MENU:
                // In the Menu, wait for the player to press ENTER to start the game.
                if (IsKeyPressed(KEY_ENTER)) {
                    // Reset game variables when starting a new game.
                    sphealX = 200; sphealY = 200;
                    orcaX = 400; orcaY = 0;
                    fishActive = false;
                    fishCollected = 0;
                    currentScreen = GAMEPLAY;
                }
                break;
            case GAMEPLAY: {
                // --- Orca Chasing Behavior ---
                float diffX = sphealX - orcaX;
                float diffY = sphealY - orcaY;
                float distance = sqrt(diffX * diffX + diffY * diffY);
                if (distance != 0) {
                    orcaX += (int)((diffX / distance) * chaseSpeed);
                    orcaY += (int)((diffY / distance) * chaseSpeed);
                }

                // --- Spheal Movement (WASD for 4-directional movement) ---
                if (IsKeyDown(KEY_D)) sphealX += 10;
                if (IsKeyDown(KEY_A)) sphealX -= 10;
                if (IsKeyDown(KEY_S)) sphealY += 10;
                if (IsKeyDown(KEY_W)) sphealY -= 10;

                // --- Clamp Spheal to Stay On-Screen ---
                float halfSphealWidth = sphealTexture.width * sphealScale / 2.0f;
                float halfSphealHeight = sphealTexture.height * sphealScale / 2.0f;
                if (sphealX < halfSphealWidth) sphealX = halfSphealWidth;
                if (sphealX > screenWidth - halfSphealWidth) sphealX = screenWidth - halfSphealWidth;
                if (sphealY < halfSphealHeight) sphealY = halfSphealHeight;
                if (sphealY > screenHeight - halfSphealHeight) sphealY = screenHeight - halfSphealHeight;

                // --- Fish Spawning ---
                // Spawn a fish if there is not an active fish.
                if (!fishActive) {
                    fishX = GetRandomValue(0, screenWidth - (int)(fishTexture.width * fishScale));
                    fishY = GetRandomValue(0, screenHeight - (int)(fishTexture.height * fishScale));
                    fishActive = true;
                }

                // --- Collision Detection for Fish Collection ---
                Rectangle sphealRect = {
                    sphealX - halfSphealWidth,
                    sphealY - halfSphealHeight,
                    sphealTexture.width * sphealScale,
                    sphealTexture.height * sphealScale
                };
                Rectangle fishRect = {
                    fishX,
                    fishY,
                    fishTexture.width * fishScale,
                    fishTexture.height * fishScale
                };
                if (fishActive && CheckCollisionRecs(sphealRect, fishRect)) {
                    fishActive = false;
                    fishCollected++;
                }

                // --- Collision Detection for Game Over (Orca hits Spheal) ---
                Rectangle orcaRect = {
                    orcaX,
                    orcaY,
                    orcaTexture.width * orcaScale,
                    orcaTexture.height * orcaScale
                };
                if (CheckCollisionRecs(sphealRect, orcaRect)) {
                    currentScreen = GAMEOVER;
                }
                break;
            }
            case GAMEOVER:
                // In Game Over, wait for the player to press R to return to the Menu.
                if (IsKeyPressed(KEY_M)) {
                    currentScreen = MENU;
                }
                break;
        }

        // Draw section
        BeginDrawing();
        ClearBackground(BLUE);

        switch (currentScreen) {
            case MENU: {
                int titleWidth = MeasureText("SPHEAL EVASION", 30);
                DrawText("SPHEAL EVASION", screenWidth/2 - titleWidth/2, screenHeight/2 - 50, 30, DARKBLUE);
                int instrWidth = MeasureText("Press ENTER to start", 20);
                DrawText("Press ENTER to start", screenWidth/2 - instrWidth/2, screenHeight/2, 20, DARKGRAY);
                break;
            }
            case GAMEPLAY: {
                // Draw Spheal (centered)
                DrawTextureEx(
                    sphealTexture,
                    (Vector2){ sphealX - (sphealTexture.width * sphealScale) / 2.0f,
                               sphealY - (sphealTexture.height * sphealScale) / 2.0f },
                    0.0f,
                    sphealScale,
                    WHITE
                );
                // Draw Orca
                DrawTextureEx(
                    orcaTexture,
                    (Vector2){ orcaX, orcaY },
                    0.0f,
                    orcaScale,
                    WHITE
                );
                // Draw Fish if active
                if (fishActive) {
                    DrawTextureEx(
                        fishTexture,
                        (Vector2){ fishX, fishY },
                        0.0f,
                        fishScale,
                        WHITE
                    );
                }
                // Draw Fish Collection Counter
                DrawText(TextFormat("Fish Collected: %i", fishCollected), 10, 10, 20, BLACK);
                break;
            }
            case GAMEOVER: {
                int gameOverWidth = MeasureText("Game Over", 30);
                DrawText("Game Over", screenWidth/2 - gameOverWidth/2, screenHeight/2 - 50, 30, RED);
                int restartWidth = MeasureText("Press R to go to Menu", 20);
                DrawText("Press M to go to Menu", screenWidth/2 - restartWidth/2, screenHeight/2, 20, DARKGRAY);
                break;
            }
        }
        EndDrawing();
    }

    // Unload textures and close window
    UnloadTexture(sphealTexture);
    UnloadTexture(orcaTexture);
    UnloadTexture(fishTexture);
    CloseWindow();

    return 0;
}
