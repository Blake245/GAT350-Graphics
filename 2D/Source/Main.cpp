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
#include "Shader.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
    // --- initialiaze ---

    // Time and Input
    Time time;
    Input input;
    input.Initialize();
    input.Update();

    // Renderer and Framebuffer
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Software Renderer", 1000, 800);

    Framebuffer framebuffer(renderer, 1000, 800);

    //Image image;
    //image.Load("../Build/Images/water.jfif");
    //PostProcess::Alpha(imageAlpha.m_buffer, 158);

    SetBlendMode(BlendMode::Normal);

    // Camera
    Camera camera(1000, 800);
    camera.SetView(glm::vec3{ 0, 3, -20 }, glm::vec3{ 0, 0, 0 });
    camera.SetProjecton(45.0f, 1000.0f / 800.0f, 0.1f, 1000.0f);
    Transform cameraTransform{ {0, 3, -20 }, {0, 0, 0 } };
    float cameraSpeed = 20.0f;

    // shader
    VertexShader::uniforms.view = camera.GetView();
    VertexShader::uniforms.projection = camera.GetProjection();
    VertexShader::uniforms.ambient = color3_t{ 0.01f };

    VertexShader::uniforms.light.position = glm::vec3{ 10, 10, -10 };
    VertexShader::uniforms.light.direction = glm::vec3{ 0, -1, 0 }; // light pointing down
    VertexShader::uniforms.light.color = color3_t{ 1, 0, 0 }; // white light

    Shader::framebuffer = &framebuffer;

#pragma region Ocean Scene
    // Model
    
    // shark
    //std::shared_ptr<Model> model = std::make_shared<Model>();
    //model->Load("../Build/Model/shark.obj");
    //model->SetColor({ 255, 255, 255, 255 });
    //// fish
    //std::shared_ptr<Model> fish = std::make_shared<Model>();
    //fish->Load("../Build/Model/fish.obj");
    //fish->SetColor({ 255, 255, 255, 255 });
    //// big fish
    //std::shared_ptr<Model> bigFish = std::make_shared<Model>();
    //bigFish->Load("../Build/Model/bigFish.obj");
    //bigFish->SetColor({ 255, 255, 255, 255 });
    ////lure
    //std::shared_ptr<Model> lure = std::make_shared<Model>();
    //lure->Load("../Build/Model/lure.obj");
    //lure->SetColor({ 255, 255, 255, 255 });

    // Actors

    //shark
    //Transform transform{ {100, 0, 0}, glm::vec3{ 10, -90, 0 }, glm::vec3{1}};
    //std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
    //actor->SetColor({ 230, 230, 230, 255 });
    //actors.push_back(std::move(actor));
    ////fish
    //Transform transformFish{ {-25, -20, 0}, glm::vec3{ 15, -80, 0 }, glm::vec3{0.5} };
    //std::unique_ptr<Actor> actorf = std::make_unique<Actor>(transformFish, fish);
    //actorf->SetColor({ 255, 255, 255, 255 });
    //actors.push_back(std::move(actorf));
    ////big fish
    //Transform t_bigFish{ {0, 100, 50}, glm::vec3{ -80, 180, 0 }, glm::vec3{1} };
    //std::unique_ptr<Actor> a_bigFish = std::make_unique<Actor>(t_bigFish, bigFish);
    //a_bigFish->SetColor({ 199, 105, 56, 255 });
    //actors.push_back(std::move(a_bigFish));
    ////lure
    //Transform t_lure{ {0, 120, 47}, glm::vec3{ 0, 0, -90 }, glm::vec3{1} };
    //std::unique_ptr<Actor> a_lure = std::make_unique<Actor>(t_lure, lure);
    //a_lure->SetColor({ 239, 73, 73, 255 });
    //actors.push_back(std::move(a_lure));

#pragma endregion


    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->Load("../Build/Model/sphere.obj");
    model->SetColor({ 0, 0, 1, 1 });

    std::vector<std::unique_ptr<Actor>> actors;


    Transform transform{ glm::vec3{ 0 }, glm::vec3{ 0 }, glm::vec3{ 5 } };
    std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
    //actor->SetColor({ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256), 255 });
    actors.push_back(std::move(actor));
    
    
    
    bool quit = false;

    // --- Update ---
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

        //framebuffer.DrawImage(0, 0, image);

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

#pragma region CameraMovement

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

            cameraTransform.position += offset * cameraSpeed * time.GetDeltaTime();
        }
        else
        {
            input.SetRelativeMode(false);
        }

        camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());
        VertexShader::uniforms.view = camera.GetView();

        for (auto& actor : actors)
        {
            actor->Draw();
        }

#pragma endregion

        framebuffer.Update();

        renderer = framebuffer;

        // --- End Frame ---
        renderer.EndFrame();
    }

    return 0;
}