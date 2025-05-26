#include "raylib.h"

/**
 *
 * Raylib (+ Raygui) VS Code Starter Template
 *
 * This assumes that raylib has been installed to the default location with the official installer.
 *
 * Hit F5 to Run/Debug the application.
 *
 * Happy Coding! :)
 *
 */

// Comment or delete the following two lines if you don't need raygui in your project
#define RAYGUI_IMPLEMENTATION
#include "deps/raygui.h"

// Include a dark style just to showcase how styles work
#include "./deps/styles/style_dark.h" // raygui style: dark

int main(void)
{

    InitWindow(1920, 1080, "Project Chicken");

    // Each style comes with its own style-loading-function.
    // Just look in the corresponding styles .h file.
    // When no style is loaded, the default white style will be used.
    GuiLoadStyleDark();

    int showMessageBox = false;

    while (!WindowShouldClose())
    {
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}