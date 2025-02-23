#include "raylib.h"
#include <math.h>  // For sqrt()

int main(void) {
    // Window dimensions
    int width = 800;
    int height = 460;
    InitWindow(width, height, "Anne's Window!");

    // Load textures (ensure these files are in your working directory)
    Texture2D sphealTexture = LoadTexture("Spheal.png");
    Texture2D orcaTexture = LoadTexture("orca-whale.png");

    // Set initial positions for the images
    int sphealX = 200;
    int sphealY = 200;
    int orcaX = 400;
    int orcaY = 0;

    // Define scale factors: spheal is smaller, orca is larger
    float sphealScale = 0.1f;
    float orcaScale = 0.5f;

    // Game over flag
    bool gameOver = false;

    // Define chase speed for the orca (smaller value means slower chasing)
    float chaseSpeed = 3.0f;

    SetTargetFPS(60); // Limits game loop to 60 fps

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // Update orca position to chase the spheal:
            float diffX = sphealX - orcaX;
            float diffY = sphealY - orcaY;
            float distance = sqrt(diffX * diffX + diffY * diffY);

            // Only update if the distance is not zero (avoid division by zero)
            if (distance != 0) {
                orcaX += (int)((diffX / distance) * chaseSpeed);
                orcaY += (int)((diffY / distance) * chaseSpeed);
            }

            // Update spheal movement with key presses (W, A, S, D for 4-directional movement)
            if (IsKeyDown(KEY_D)) {
                sphealX += 10;
            }
            if (IsKeyDown(KEY_A)) {
                sphealX -= 10;
            }
            if (IsKeyDown(KEY_S)) {
                sphealY += 10;
            }
            if (IsKeyDown(KEY_W)) {
                sphealY -= 10;
            }

            // Calculate half the width and height of the spheal texture after scaling
            float halfSphealWidth = sphealTexture.width * sphealScale / 2.0f;
            float halfSphealHeight = sphealTexture.height * sphealScale / 2.0f;

            // Clamp spheal's position so it stays completely on-screen horizontally
            if (sphealX < halfSphealWidth) {
                sphealX = halfSphealWidth;
            }
            if (sphealX > width - halfSphealWidth) {
                sphealX = width - halfSphealWidth;
            }

            // Clamp spheal's position so it stays completely on-screen vertically
            if (sphealY < halfSphealHeight) {
                sphealY = halfSphealHeight;
            }
            if (sphealY > height - halfSphealHeight) {
                sphealY = height - halfSphealHeight;
            }

            // Create rectangles for collision detection based on scaled dimensions
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

            // Check collision: if it happens, set gameOver to true
            if (CheckCollisionRecs(sphealRect, orcaRect)) {
                gameOver = true;
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);

        if (gameOver) {
            int textWidth = MeasureText("Game Over", 20);
            DrawText("Game Over", width / 2 - textWidth / 2, height / 2, 20, RED);
        } else {
            // Draw the spheal texture, centered at (sphealX, sphealY)
            DrawTextureEx(
                sphealTexture,
                (Vector2){ sphealX - (sphealTexture.width * sphealScale) / 2.0f,
                           sphealY - (sphealTexture.height * sphealScale) / 2.0f },
                0.0f,
                sphealScale,
                WHITE
            );

            // Draw the orca texture at its current position
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

    // Unload textures and close window
    UnloadTexture(sphealTexture);
    UnloadTexture(orcaTexture);
    CloseWindow();

    return 0;
}
