#include "Renderer.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer renderer;

    renderer.Initialize();

    renderer.CreateWindow("2D", 800, 600);

    while (true)
    {
        // clear screen
        renderer.ClearFrame();

        // show screen
        renderer.PresentFrame();
    }

    return 0;
}