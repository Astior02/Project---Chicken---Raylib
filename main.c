#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "deps/raygui.h"
#include "./deps/styles/style_dark.h" 

int main(void)
{
    const int tamanhoTelaX = 1920;
    const int tamanhoTelaY = 1080;
    InitWindow(tamanhoTelaX, tamanhoTelaY, "Project Chicken");
    
    

    

    while (!WindowShouldClose())
    {
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}