#include "raylib.h"
#include <math.h>   // For sqrt()

int main(void) {
    // Window dimensions
    int width = 800;
    int height = 460;
    InitWindow(width, height, "Anne's Window!");

    // Load textures (make sure these files are in your working directory)
    Texture2D sphealTexture = LoadTexture("Spheal.png");
    Texture2D orcaTexture = LoadTexture("orca-whale.png");
    Texture2D fishTexture   = LoadTexture("fish.png");  // Your fish image

    // Scale factors for each image
    float sphealScale = 0.1f; // spheal image is small
    float orcaScale   = 0.5f; // orca image is larger
    float fishScale   = 0.3f; // adjust as needed for fish size

    // Initial positions for spheal and orca
    int sphealX = 200;
    int sphealY = 200;
    int orcaX = 400;
    int orcaY = 0;
    float chaseSpeed = 3.0f;  // Speed at which orca chases spheal

    // Variables for fish collectible
    int fishX = 0;
    int fishY = 0;
    bool fishActive = false;  // Indicates whether a fish is currently spawned
    int fishCollected = 0;    // Count of collected fish

    // Game over flag
    bool gameOver = false;

    SetTargetFPS(60); // Limits game loop to 60 fps

    while (!WindowShouldClose()) {

        if (!gameOver) {

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
            float halfSphealWidth  = sphealTexture.width * sphealScale / 2.0f;
            float halfSphealHeight = sphealTexture.height * sphealScale / 2.0f;
            if (sphealX < halfSphealWidth) sphealX = halfSphealWidth;
            if (sphealX > width - halfSphealWidth) sphealX = width - halfSphealWidth;
            if (sphealY < halfSphealHeight) sphealY = halfSphealHeight;
            if (sphealY > height - halfSphealHeight) sphealY = height - halfSphealHeight;

            // --- Fish Spawning ---
            // If there is no active fish, spawn one at a random location
            if (!fishActive) {
                // Ensure the fish is fully on-screen by subtracting the scaled size
                fishX = GetRandomValue(0, width - (int)(fishTexture.width * fishScale));
                fishY = GetRandomValue(0, height - (int)(fishTexture.height * fishScale));
                fishActive = true;
            }

            // --- Collision Detection for Fish Collection ---
            // Create rectangle for spheal (centered)
            Rectangle sphealRect = {
                sphealX - halfSphealWidth,
                sphealY - halfSphealHeight,
                sphealTexture.width * sphealScale,
                sphealTexture.height * sphealScale
            };
            // Create rectangle for fish (top-left positioned)
            Rectangle fishRect = {
                fishX,
                fishY,
                fishTexture.width * fishScale,
                fishTexture.height * fishScale
            };

            // If spheal collects the fish, deactivate fish and increment counter
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
                gameOver = true;
            }
        } else {
            // --- Restart Game: Press 'R' to reset ---
            if (IsKeyPressed(KEY_R)) {
                // Reset positions
                sphealX = 200;
                sphealY = 200;
                orcaX = 400;
                orcaY = 0;
                // Reset fish collectible
                fishActive = false;
                fishCollected = 0;
                // Reset game over flag
                gameOver = false;
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);

        if (gameOver) {
            int gameOverWidth = MeasureText("Game Over", 20);
            DrawText("Game Over", width/2 - gameOverWidth/2, height/2 - 20, 20, RED);
            int restartWidth = MeasureText("Press R to Restart", 20);
            DrawText("Press R to Restart", width/2 - restartWidth/2, height/2 + 10, 20, RED);
        } else {
            // --- Draw Spheal ---
            DrawTextureEx(
                sphealTexture,
                (Vector2){ sphealX - (sphealTexture.width * sphealScale) / 2.0f,
                           sphealY - (sphealTexture.height * sphealScale) / 2.0f },
                0.0f,
                sphealScale,
                WHITE
            );

            // --- Draw Orca ---
            DrawTextureEx(
                orcaTexture,
                (Vector2){ orcaX, orcaY },
                0.0f,
                orcaScale,
                WHITE
            );

            // --- Draw Fish if Active ---
            if (fishActive) {
                DrawTextureEx(
                    fishTexture,
                    (Vector2){ fishX, fishY },
                    0.0f,
                    fishScale,
                    WHITE
                );
            }

            // --- Draw Fish Counter at the Top ---
            DrawText(TextFormat("Fish Collected: %i", fishCollected), 10, 10, 20, BLACK);
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
