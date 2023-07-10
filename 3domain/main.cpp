
#include <iostream>
#include <list>
#include <filesystem>
#include <raylib.h>
#include <raymath.h>
#include "raygui.h"
#include "Domain.h"

int main(int argc, char* argv[])
{
    const int minScreenWidth = 640;
    const int minScreenHeight = 480;
    int screenWidth = minScreenWidth;
    int screenHeight = minScreenHeight;
    if (argc == 3)
    {
        int i = atoi(argv[1]);
        if (i > minScreenWidth) screenWidth = i;
        i = atoi(argv[2]);
        if (i > minScreenHeight) screenHeight = i;
    }
    Image ico = LoadImage("icon.png");
    

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Domain");
    SetWindowMinSize(minScreenWidth, minScreenHeight);
    SetWindowIcon(ico);
    SetExitKey(KEY_NULL);
    
    Domain app;

    std::cout <<"INFO: Working directory: " << GetWorkingDirectory() << "\n";

    while (!WindowShouldClose())
    {
        app.Update();
        BeginDrawing();
        app.Draw();
        EndDrawing();
    }
    UnloadImage(ico);
    CloseWindow();
}

