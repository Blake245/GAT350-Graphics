#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Model.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 800, 600);

    Framebuffer framebuffer(renderer, 800, 600);

    Image image;
    image.Load("../Build/Images/test.png");

    Image image2;
    image2.Load("../Build/Images/pixel.jpeg");

    Image imageAlpha;
    imageAlpha.Load("../Build/Images/colors.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 158);

    SetBlendMode(BlendMode::Normal);


    vertices_t vertices{ {-5, 5, 0}, {5, 5, 0}, {-5, 0, 0} };
    Model model(vertices, { 0, 255, 0, 255 });

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
        
        // --- Start Frame ---
        framebuffer.Clear(color_t{ 0, 0, 0, 255 });

#pragma region shapes

        SetBlendMode(BlendMode::Normal);
        framebuffer.DrawImage(100, 50, image);

        framebuffer.DrawRect(200, 100, 100, 100, { 47, 204, 178, 255});

        SetBlendMode(BlendMode::Alpha);
        framebuffer.DrawRect(500, 100, 150, 100, { 47, 204, 178, 100});

        SetBlendMode(BlendMode::Additive);
        framebuffer.DrawRect(200, 300, 150, 100, { 47, 204, 178, 100});

        SetBlendMode(BlendMode::Multiply);
        framebuffer.DrawRect(500, 300, 100, 100, { 47, 204, 178, 100});

        /*framebuffer.DrawTriangle(500, 250, 675, 50, 700, 350, { 47, 204, 178, 255 });

        framebuffer.DrawLine(100, 450, 600, 500, { 65, 207, 9, 255 });

        framebuffer.DrawCircle(200, 200, 100, { 200, 10, 255, 255 });*/
#pragma endregion

#pragma region AlphaBlend

        //int mx, my;
        //SDL_GetMouseState(&mx, &my);

        ///*SetBlendMode(BlendMode::Normal);
        //framebuffer.DrawImage(200, 50, image);*/

        ////framebuffer.DrawImage(100, 350, image2);

        //SetBlendMode(BlendMode::Alpha);
        //framebuffer.DrawImage(mx - (imageAlpha.m_width / 2), my - (imageAlpha.m_height / 2), imageAlpha);
#pragma endregion

        

#pragma region Curves
        ////framebuffer.DrawLinearCurve(100, 100, 250, 200, { 0, 255, 0, 255 });

        //framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 0, 0, 255, 255 });

        //framebuffer.DrawCubicCurve(300, 400, 300, 100, mx, my, 600, 400, { 255, 0, 0, 255 });

        //int ticks = SDL_GetTicks();
        //float time = ticks * 0.001f;
        //float t = std::abs(std::sin(time));
        //int x, y;
        //CubicPoint(300, 400, 300, 100, mx, my, 600, 400, t, x, y);
        //framebuffer.DrawRect(x, y, 40, 40, { 47, 204, 178, 255 });
        //framebuffer.DrawCircle(x, y, 40, { 255, 204, 178, 255 });

#pragma endregion



#pragma region post_process
        //post process
        //PostProcess::Invert(framebuffer.m_buffer);
        //PostProcess::MonoChome(framebuffer.m_buffer);
        //PostProcess::Brightness(framebuffer.m_buffer, 40);
        //PostProcess::ColorBalance(framebuffer.m_buffer, 150, -50, -50);
        //PostProcess::Noise(framebuffer.m_buffer, 80);
        //PostProcess::Threshold(framebuffer.m_buffer, 75);
        //PostProcess::Posterize(framebuffer.m_buffer, 6);

        //for (int i = 0; i < 5; i++)
        //{
        //    //PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //    PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //    
        //}
        //PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 10);
        //PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::MonoChome(framebuffer.m_buffer);
#pragma endregion

#pragma region Matrix
        /*int ticks = SDL_GetTicks(); 
        float time = ticks * 0.001f;

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 translate = glm::translate(modelMatrix, glm::vec3(350.0f, 240.0f, 0.0f));
        glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(10));
        glm::mat4 rotate = glm::rotate(modelMatrix, glm::radians(time * 180.0f), glm::vec3{1, 1, 0});

        modelMatrix = translate * scale * rotate;

        model.Draw(framebuffer, modelMatrix);*/

#pragma endregion


        framebuffer.Update();

        renderer = framebuffer;


        // --- End Frame ---
        renderer.EndFrame();
    }

    return 0;
}