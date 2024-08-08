#include <iostream>
#include "raylib.h"

using namespace std;

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    const int squareWidth = screenWidth/8;
    int inc = screenWidth/8;

    InitWindow(screenWidth, screenHeight, "Chessmate");

    Image bk = LoadImageSvg("./pieces/black/king.svg", inc, inc);
    Texture2D texture = LoadTextureFromImage(bk);
    UnloadImage(bk);

    SetTargetFPS(60); 
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Board
        for (int i = 0; i <= screenWidth; i+= screenWidth/8) {
            int j = (i%2==0) ? inc : 0;
            for (; j <= screenWidth; j += inc*2) {
                DrawRectangle(j, i, squareWidth, squareWidth, DARKPURPLE);
            }

            j = (i%2==0) ? 0 : inc;
            for (; j <= screenWidth; j += inc*2) {
                DrawRectangle(j, i, squareWidth, squareWidth, RAYWHITE);
            }
        }

        DrawTexture(texture, inc, inc, WHITE);
        EndDrawing();
    }

    UnloadTexture(texture);

    CloseWindow(); 

    return 0;
}
