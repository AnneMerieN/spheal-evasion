#include "raylib.h"
#include <math.h>

int main(void) {
    // Window dimensions
    int width = 800;
    int height = 460;
    InitWindow(width, height, "Anne's Window!");

    // Load textures (make sure these files are in your working directory)
    Texture2D sphealTexture = LoadTexture("Spheal.png");
    Texture2D orcaTexture = LoadTexture("orca-whale.png");

    // Game state variables (initial values)
    int sphealX = 200;
    int sphealY = 200;
    int orcaX = 400;
    int orcaY = 0;
    float chaseSpeed = 3.0f;  // Adjust this for chasing speed
    bool gameOver = false;

    // Scale factors for each image
    float sphealScale = 0.1f; // spheal image is small
    float orcaScale = 0.5f;   // orca image is larger

    SetTargetFPS(60); // Limits game loop to 60 fps

    while (!WindowShouldClose()) {
        // If game is not over, update game logic
        if (!gameOver) {
            // Orca chasing behavior: Calculate direction toward the spheal
            float diffX = sphealX - orcaX;
            float diffY = sphealY - orcaY;
            float distance = sqrt(diffX * diffX + diffY * diffY);
            if (distance != 0) {
                orcaX += (int)((diffX / distance) * chaseSpeed);
                orcaY += (int)((diffY / distance) * chaseSpeed);
            }

            // Update spheal movement using WASD (or A/D for horizontal, W/S for vertical)
            if (IsKeyDown(KEY_D)) sphealX += 10;
            if (IsKeyDown(KEY_A)) sphealX -= 10;
            if (IsKeyDown(KEY_S)) sphealY += 10;
            if (IsKeyDown(KEY_W)) sphealY -= 10;

            // Calculate half dimensions for clamping (based on spheal's scaled size)
            float halfSphealWidth = sphealTexture.width * sphealScale / 2.0f;
            float halfSphealHeight = sphealTexture.height * sphealScale / 2.0f;

            // Clamp spheal's horizontal position
            if (sphealX < halfSphealWidth) sphealX = halfSphealWidth;
            if (sphealX > width - halfSphealWidth) sphealX = width - halfSphealWidth;
            // Clamp spheal's vertical position
            if (sphealY < halfSphealHeight) sphealY = halfSphealHeight;
            if (sphealY > height - halfSphealHeight) sphealY = height - halfSphealHeight;

            // Create rectangles for collision detection (using scaled dimensions)
            Rectangle sphealRect = {
                sphealX - halfSphealWidth,
                sphealY - halfSphealHeight,
                sphealTexture.width * sphealScale,
                sphealTexture.height * sphealScale
            };
            Rectangle orcaRect = {
                orcaX,
                orcaY,
                orcaTexture.width * orcaScale,
                orcaTexture.height * orcaScale
            };

            // Check for collision between spheal and orca
            if (CheckCollisionRecs(sphealRect, orcaRect)) {
                gameOver = true;
            }
        } else {
            // If the game is over, allow the user to press 'R' to restart
            if (IsKeyPressed(KEY_R)) {
                // Reset game state variables to initial conditions
                sphealX = 200;
                sphealY = 200;
                orcaX = 400;
                orcaY = 0;
                gameOver = false;
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);

        if (gameOver) {
            // Display game over message and restart instructions
            int textWidth = MeasureText("Game Over", 20);
            DrawText("Game Over", width / 2 - textWidth / 2, height / 2 - 20, 20, RED);
            int restartTextWidth = MeasureText("Press R to Restart", 20);
            DrawText("Press R to Restart", width / 2 - restartTextWidth / 2, height / 2 + 10, 20, RED);
        } else {
            // Draw the spheal texture, centered at (sphealX, sphealY) with scaling
            DrawTextureEx(
                sphealTexture,
                (Vector2){ sphealX - (sphealTexture.width * sphealScale) / 2.0f,
                           sphealY - (sphealTexture.height * sphealScale) / 2.0f },
                0.0f,
                sphealScale,
                WHITE
            );

            // Draw the orca texture at its current position with scaling
            DrawTextureEx(
                orcaTexture,
                (Vector2){ orcaX, orcaY },
                0.0f,
                orcaScale,
                WHITE
            );
        }

        EndDrawing();
    }

    // Unload textures and close the window
    UnloadTexture(sphealTexture);
    UnloadTexture(orcaTexture);
    CloseWindow();

    return 0;
}
