#include "raylib.h"

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
    int orcaDirection = 10;

    // Define scale factors: spheal is smaller, orca is larger
    float sphealScale = 0.1f;
    float orcaScale = 0.5f;

    // Game over flag
    bool gameOver = false;

    SetTargetFPS(60); // Limits game loop to 60 fps

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // Update orca movement
            orcaY += orcaDirection;
            if (orcaY > height - orcaTexture.height * orcaScale || orcaY < 0) {
                orcaDirection = -orcaDirection;
            }

            // Update spheal movement
            if (IsKeyDown(KEY_D)) {
                sphealX += 10;
            }
            if (IsKeyDown(KEY_A)) {
                sphealX -= 10;
            }

            // Calculate half the width of the spheal texture (scaled)
            float halfSphealWidth = sphealTexture.width * sphealScale / 2.0f;

            // Clamp sphealX so the spheal stays completely on-screen
            if (sphealX < halfSphealWidth) {
                sphealX = halfSphealWidth;
            }
            if (sphealX > width - halfSphealWidth) {
                sphealX = width - halfSphealWidth;
            }

            // Create rectangles for collision detection
            Rectangle sphealRect = {
                sphealX - halfSphealWidth,
                sphealY - (sphealTexture.height * sphealScale / 2.0f),
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
            // Display the permanent game over screen
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

            // Draw the orca texture
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
