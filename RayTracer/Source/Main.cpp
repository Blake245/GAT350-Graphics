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
#include "Tracer.h"
#include "Scene.h"
#include "Plane.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    // initialize
    Time time;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Ray Tracer", 1000, 800);

    SetBlendMode(BlendMode::Normal);

    Framebuffer framebuffer(renderer, 1000, 800);

    Camera camera{70.0f, framebuffer.m_width / (float)framebuffer.m_height};
    camera.SetView({ 0, 0, -50 }, { 0,0,0 });

    Scene scene;


    std::shared_ptr<Material> gray  = std::make_shared<Lambertian>(color3_t{ 0.5f });
    std::shared_ptr<Material> red   = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    std::shared_ptr<Material> blue  = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
    std::shared_ptr<Material> green  = std::make_shared<Lambertian>(color3_t{ 0, 1, 0 });
    std::shared_ptr<Material> purple  = std::make_shared<Lambertian>(color3_t{ 1, 0, 1 });

    std::array<std::shared_ptr<Material>, 5> material = {gray, red, blue, green, purple};

   // std::shared_ptr<Material> material = std::make_shared<Material>(color3_t{ 1, 0, 0 });
    /*auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 5, 0 }, 10.0f, red);

    scene.AddObject(std::move(sphere));*/

    //std::shared_ptr<Material> material2 = std::make_shared<Material>(color3_t{ 0, 0, 1 });
    auto plane = std::make_unique<Plane>(glm::vec3{ 0, -5, 0 }, glm::vec3{ 0, 1, 0 }, gray);
    scene.AddObject(std::move(plane));

    for (int i = 0; i < 20; i++)
    {
        int matnum = random(5);
        auto sphere = std::make_unique<Sphere>(random(glm::vec3(-30, -5, -30), glm::vec3(30)), randomf(1, 5), material[matnum]);
        //auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 2.0f, red);
        scene.AddObject(std::move(sphere));
    }
    
    bool quit = false;

    while (!quit)
    {
        time.Tick();

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

        
        // --- Start Frame ---
        framebuffer.Clear(ColorConvert(color4_t{ 0, 0.25f, 0, 1 }));

        scene.Render(framebuffer, camera);

        framebuffer.Update();
        // --- End Frame ---

        renderer = framebuffer;
        renderer.EndFrame();
    }

    return 0;
}