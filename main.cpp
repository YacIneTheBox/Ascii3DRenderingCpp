#include "raylib.h"
#include "rcamera.h"
#include <cstdlib>
#include <ctime>

/*
 * 1 - creation of the 3d world
 */

using namespace std;

#define MAX_COLUMNS 20

int main(void){
    srand(static_cast<unsigned int>(std::time(nullptr)));
    const int ScreenWidth = 1200;
    const int ScreenHeight = 800;

    InitWindow(ScreenWidth, ScreenHeight, "Ascii3DRenderingCpp");

    Camera camera = {0};
    camera.position = (Vector3){0.0f,2.0f,4.0f};// pos
    camera.target = (Vector3){0.0f,0.0f,0.0f};// target
    camera.up = (Vector3){0.0f,1.0f,0.0f};// up
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    int cameraMode = CAMERA_FIRST_PERSON;

    float heights[MAX_COLUMNS] = {0};
    Vector3 positions[MAX_COLUMNS] = {0};
    Color colors[MAX_COLUMNS] = {0};
    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
        colors[i] = (Color){ 0, 0, 30, 255 };
    }

    SetTargetFPS(60);

    DisableCursor();

    while (!WindowShouldClose()){
        UpdateCamera(&camera,cameraMode);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawPlane((Vector3){0.0f,0.0f,0.0f}, (Vector2){20.0f,20.0f}, LIGHTGRAY);
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);

                for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    DrawCube(positions[i], 1.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeWires(positions[i], 1.0f, heights[i], 2.0f, WHITE);
                }

                EndMode3D();

        EndDrawing();
    }

    CloseWindow();
}
