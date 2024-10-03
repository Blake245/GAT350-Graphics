#include "Renderer.h"
#include "Framebuffer.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 800, 600);

    Framebuffer framebuffer(renderer, 200, 150);

    bool quit = false;

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }

        // clear screen
        renderer.ClearFrame();

        framebuffer.Clear(color_t{ 0, 0, 0, 255 });


        for (int i = 0; i < 100; i++)
        {
            int x = rand() % 200;
            int y = rand() % 150;
            framebuffer.DrawPoint(x, y, color_t{ 255, 0, 255, 255 });
        }

        framebuffer.DrawRect(10, 10, 150, 100, { 47, 204, 178, 255});

        framebuffer.Update();

        renderer = framebuffer;


        // show screen
        renderer.PresentFrame();
    }

    return 0;
}