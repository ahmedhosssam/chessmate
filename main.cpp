#include <iostream>
#include "raylib.h"

using namespace std;

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Chessmate");

    SetTargetFPS(60); 
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw vertical lines
        for (int i = 0; i <= screenWidth; i+= screenWidth/8) {
            //DrawLine(100, 0, 100, 1000, BLACK);
            DrawLine(i, 0, i, 1000, BLACK);
        }

        // Draw horizontal lines
        for (int i = 0; i <= screenWidth; i+= screenWidth/8) {
            //DrawLine(100, 0, 100, 1000, BLACK);
            DrawLine(0, i, 1000, i, BLACK);
        }

        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}
