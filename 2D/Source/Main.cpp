#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
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


        for (int i = 0; i < 0; i++)
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
        /*int x = rand() % framebuffer.m_width;
        int y = rand() % framebuffer.m_height;
        int x2 = rand() % framebuffer.m_width;
        int y2 = rand() % framebuffer.m_height;
        int x3 = rand() % framebuffer.m_width;
        int y3 = rand() % framebuffer.m_height;*/

        //framebuffer.DrawRect(300, 300, 100, 100, { 47, 204, 178, 255});

        //framebuffer.DrawTriangle(500, 250, 675, 50, 700, 350, { 47, 204, 178, 255 });

        //framebuffer.DrawLine(100, 450, 600, 500, { 65, 207, 9, 255 });

        //framebuffer.DrawCircle(200, 200, 100, { 200, 10, 255, 255 });

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        //framebuffer.DrawLinearCurve(100, 100, 250, 200, { 0, 255, 0, 255 });

        framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 0, 0, 255, 255 });

        framebuffer.DrawCubicCurve(300, 400, 300, 100, mx, my, 600, 400, { 255, 0, 0, 255 });

        int ticks = SDL_GetTicks();
        float time = ticks * 0.001f;
        float t = std::abs(std::sin(time));
        int x, y;
        CubicPoint(300, 400, 300, 100, mx, my, 600, 400, t, x, y);
        framebuffer.DrawRect(x, y, 40, 40, { 47, 204, 178, 255 });

        framebuffer.Update();

        renderer = framebuffer;


        // show screen
        renderer.EndFrame();
    }

    return 0;
}