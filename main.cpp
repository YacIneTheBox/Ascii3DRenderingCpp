#include "raylib.h"
using namespace std;

typedef struct Body{
    Vector3 position;
    Vector3 rotation;
    Vector3 velocity;
    bool isGrounded;
};

int main(void){
    const int ScreenWidth = 1200;
    const int ScreenHeight = 800;

    InitWindow(ScreenWidth, ScreenHeight, "Ascii3DRenderingCpp");

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
}
