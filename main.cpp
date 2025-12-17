#include "raylib.h"
#include "rcamera.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

#define MAX_COLUMNS 20

int main(void){
    srand(static_cast<unsigned int>(std::time(nullptr)));
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;

    InitWindow(ScreenWidth, ScreenHeight, "Ascii3DRenderingCpp");

    const int asciiWidth = 1280;
    const int asciiHeight = 720;
    RenderTexture2D target = LoadRenderTexture(asciiWidth, asciiHeight);

    // MÉTHODE ALTERNATIVE : Créer une texture de font plus simple
    const char* asciiChars = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    int charCount = strlen(asciiChars);

    // Créer l'image AVANT d'essayer de dessiner dessus
    int charWidth = 8;
    int charHeight = 10;
    Image fontImage = GenImageColor(charCount * charWidth, charHeight, BLACK);

    // Vérifier que l'image est valide
    if (fontImage.data == NULL) {
        TraceLog(LOG_ERROR, "Failed to create font image");
        CloseWindow();
        return -1;
    }

    // Dessiner chaque caractère
    for (int i = 0; i < charCount; i++)
    {
        Vector2 pos = {(float)(i * charWidth), 0.0f};
        char str[2] = {asciiChars[i], '\0'};
        ImageDrawText(&fontImage, str, (int)pos.x, (int)pos.y, 10, WHITE);
    }

    Texture2D fontTexture = LoadTextureFromImage(fontImage);
    UnloadImage(fontImage);

    // Vérifier que la texture est valide
    if (fontTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load font texture");
        CloseWindow();
        return -1;
    }

    // Charger le shader
    Shader ascii = LoadShader(0, "Ressources/ascii_simple.fs");

    // Vérifier que le shader est valide
    if (ascii.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load shader");
        UnloadTexture(fontTexture);
        UnloadRenderTexture(target);
        CloseWindow();
        return -1;
    }

    // Passer les uniforms au shader
    int fontTexLoc = GetShaderLocation(ascii, "fontTexture");
    int resolutionLoc = GetShaderLocation(ascii, "resolution");

    Vector2 resolution = {(float)asciiWidth, (float)asciiHeight};
    SetShaderValue(ascii, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);

    // Setup caméra et scène 3D
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, 4.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
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
        colors[i] = (Color){ 255, 30, 255, 255 };
    }

    SetTargetFPS(60);
    DisableCursor();

    while (!WindowShouldClose()){
        UpdateCamera(&camera, cameraMode);

        // Rendre la scène 3D dans la texture
        BeginTextureMode(target);
            ClearBackground(BLACK);
            BeginMode3D(camera);
                DrawPlane((Vector3){0.0f,0.0f,0.0f}, (Vector2){32.0f,32.0f}, LIGHTGRAY);
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);

                for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, WHITE);
                }
            EndMode3D();
        EndTextureMode();

        // Appliquer le shader ASCII
        BeginDrawing();
            ClearBackground(BLACK);

            BeginShaderMode(ascii);
                SetShaderValueTexture(ascii, fontTexLoc, fontTexture);

                Rectangle src = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
                Rectangle dst = { 0, 0, (float)ScreenWidth, (float)ScreenHeight };
                DrawTexturePro(target.texture, src, dst, {0,0}, 0.0f, WHITE);
            EndShaderMode();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(fontTexture);
    UnloadRenderTexture(target);
    UnloadShader(ascii);
    CloseWindow();

    return 0;
}
