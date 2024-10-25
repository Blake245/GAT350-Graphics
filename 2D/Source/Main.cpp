#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Actor.h"
#include "Random.h"
#include "Camera.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
    // initialiaze
    Time time;
    Input input;
    input.Initialize();
    input.Update();

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Software Renderer", 1000, 800);

    Framebuffer framebuffer(renderer, 1000, 800);

    Image image;
    image.Load("../Build/Images/test.png");

    Image image2;
    image2.Load("../Build/Images/pixel.jpeg");

    Image imageAlpha;
    imageAlpha.Load("../Build/Images/colors.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 158);

    SetBlendMode(BlendMode::Normal);

    Camera camera(1000, 800);
    camera.SetView(glm::vec3{ 0, 0, -73 }, glm::vec3{ 0, 0, 0 });
    camera.SetProjecton(45.0f, 1000.0f / 800.0f, 0.1f, 1000.0f);
    Transform cameraTransform{ {0, 0, 0 }, {0, 180, 0 } };

    //vertices_t vertices{ {-5, 5, 0}, {5, 5, 0}, {-5, -5, 0} };
    //Model model(vertices, { 0, 255, 0, 255 });

    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->Load("../Build/Model/sphere.obj");
    model->SetColor({ 255, 255, 255, 255 });
    
    //std::vector<std::unique_ptr<Actor>> actors;

    std::vector<std::unique_ptr<Actor>> actors;
    for (int i = 0; i < 3; i++)
    {
        Transform transform{ {randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f)}, glm::vec3{ 0, 0, 0 }, glm::vec3{ randomf(1.0f, 2.0f) }};
        std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
        actor->SetColor({ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256), 255 });
        actors.push_back(std::move(actor));

        std::cout << transform.position.x << transform.position.y << transform.position.z <<std::endl;
    }

    /*Model sword;
    sword.Load("../Build/Model/sword.obj");
    Transform swordT { {200, 400, 0}, glm::vec3{ 60, 55, 45 }, glm::vec3{ 14 } };*/
    
    bool quit = false;
    //int timer = 5000;

    while (!quit)
    {
        time.Tick();
        input.Update();

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

        /*SetBlendMode(BlendMode::Normal);
        framebuffer.DrawImage(100, 50, image);

        framebuffer.DrawRect(200, 100, 100, 100, { 47, 204, 178, 100});

        SetBlendMode(BlendMode::Alpha);
        framebuffer.DrawRect(500, 100, 150, 100, { 47, 204, 178, 100});

        SetBlendMode(BlendMode::Additive);
        framebuffer.DrawRect(200, 300, 150, 100, { 47, 204, 178, 100});

        SetBlendMode(BlendMode::Multiply);
        framebuffer.DrawRect(500, 300, 100, 100, { 47, 204, 178, 100});*/

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

        if (input.GetMouseButtonDown(2))
        {
            input.SetRelativeMode(true);

            glm::vec3 direction{ 0 };
            if (input.GetKeyDown(SDL_SCANCODE_D)) direction.x = 1;
            if (input.GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;

            if (input.GetKeyDown(SDL_SCANCODE_E)) direction.y = 1;
            if (input.GetKeyDown(SDL_SCANCODE_Q)) direction.y = -1;

            if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = 1;
            if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -1;

            cameraTransform.rotation.y += input.GetMouseRelative().x * 0.35f;
            cameraTransform.rotation.x += input.GetMouseRelative().y * 0.35f;

            glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

            cameraTransform.position += offset * 70.0f * time.GetDeltaTime();
            std::cout << std::endl;
        }
        else
        {
            input.SetRelativeMode(false);
        }

        camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());



        //transform.position += direction * 700.0f * time.GetDeltaTime();
        ////transform.rotation.z += 180 * time.GetDeltaTime();
        //transform.rotation.z += direction.z;
        //transform.rotation.y += rotation.y;

        //model.SetColor({ 0, 255, 0, 255 });
        //model->Draw(framebuffer, transform.GetMatrix());

        for (auto& actor : actors)
        {
            actor->Draw(framebuffer, camera);
        }

        /*sword.SetColor({ 255, 0, 0, 255 });
        sword.Draw(framebuffer, swordT.GetMatrix(), camera);*/

#pragma endregion

        framebuffer.Update();

        renderer = framebuffer;

       // timer -= time.GetDeltaTime() * 1000;
        // --- End Frame ---
        renderer.EndFrame();
    }

    return 0;
}