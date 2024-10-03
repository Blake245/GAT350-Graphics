#include "Renderer.h"
#include "Framebuffer.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 800, 600);

    Framebuffer framebuffer(renderer, 800, 600);

    bool quit = false;

    while (!quit)
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
        //renderer.StartFrame();

        framebuffer.Clear(color_t{ 0, 0, 0, 255 });


        for (int i = 0; i < 100; i++)
        {
            int x = rand() % framebuffer.m_width;
            int y = rand() % framebuffer.m_height;
            int x2 = rand() % framebuffer.m_width;
            int y2 = rand() % framebuffer.m_height;

            uint8_t r = rand() % 255;
            uint8_t g = rand() % 255;
            uint8_t b = rand() % 255;

            //framebuffer.DrawPoint(x, y, color_t{ 255, 0, 255, 255 });
            //framebuffer.DrawLine(x, y, x2, y2, { 255, 0, 255, 255 });
            //framebuffer.DrawLine(x, y, x2, y2, color_t{ r, g, b, 255 });
        }
        int x = rand() % framebuffer.m_width;
        int y = rand() % framebuffer.m_height;
        int x2 = rand() % framebuffer.m_width;
        int y2 = rand() % framebuffer.m_height;
        int x3 = rand() % framebuffer.m_width;
        int y3 = rand() % framebuffer.m_height;

        framebuffer.DrawRect(400, 300, 150, 100, { 47, 204, 178, 255});

        //framebuffer.DrawTriangel(x, y, x2, y2, x3, y3, { 47, 204, 178, 255 });

        framebuffer.Update();

        renderer = framebuffer;


        // show screen
        renderer.EndFrame();
    }

    return 0;
}