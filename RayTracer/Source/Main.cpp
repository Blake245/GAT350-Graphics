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
    camera.SetView({ 0, 20, -50}, { 0,0,0 });

    Scene scene;


    std::shared_ptr<Material> gray          = std::make_shared<Lambertian>(color3_t{ 0.5f });
    std::shared_ptr<Material> red           = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    std::shared_ptr<Material> blue          = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
    std::shared_ptr<Material> green         = std::make_shared<Lambertian>(color3_t{ 0, 1, 0 });
    std::shared_ptr<Material> purple        = std::make_shared<Lambertian>(color3_t{ 1, 0, 1 });

    std::shared_ptr<Material> redMetal      = std::make_shared<Metal>(color3_t{ 1, 0, 0 }, 0);
    std::shared_ptr<Material> blueMetal     = std::make_shared<Metal>(color3_t{ 0, 0, 1 }, 0);
    std::shared_ptr<Material> greenMetal    = std::make_shared<Metal>(color3_t{ 0, 1, 0 }, 0);
    std::shared_ptr<Material> purpleMetal   = std::make_shared<Metal>(color3_t{ 1, 0, 1 }, 0);

    std::shared_ptr<Material> clear = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.33f);
    
    std::shared_ptr<Material> redLight = std::make_shared<Emissive>(color3_t{ 1, 0, 0 }, 8.0f);
    std::shared_ptr<Material> blueLight = std::make_shared<Emissive>(color3_t{ 0.2f, 0.92f, 0.88f }, 3.0f);
    std::shared_ptr<Material> yellowLight = std::make_shared<Emissive>(color3_t{ 1, 1, 0 }, 10.0f);

    std::array<std::shared_ptr<Material>, 12> materials = {red, blue, green, purple, 
        redMetal, blueMetal, greenMetal, purpleMetal, 
        clear, redLight, blueLight, yellowLight };

    int matnum1 = random(9);
    //auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 5, 0 }, 10.0f, materials[matnum1]);
    auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 5, 0 }, 10.0f, clear);
    scene.AddObject(std::move(sphere));

    //std::shared_ptr<Material> material2 = std::make_shared<Material>(color3_t{ 0, 0, 1 });
    auto plane = std::make_unique<Plane>(glm::vec3{ 0, -5, 0 }, glm::vec3{ 0, 1, 0 }, gray);
    scene.AddObject(std::move(plane));

    for (int i = 0; i < 7; i++)
    {
        int matnum = random(12);
        auto sphere = std::make_unique<Sphere>(random(glm::vec3(-30, -5, -40), glm::vec3(30)), randomf(1, 7), materials[matnum]);
        //auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 2.0f, red);
        scene.AddObject(std::move(sphere));
    }
    
    //framebuffer.Clear(ColorConvert(color4_t{ 0, 0.25f, 0, 1 }));
    scene.Render(framebuffer, camera, 1000, 3);
    framebuffer.Update();

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
        /*framebuffer.Clear(ColorConvert(color4_t{ 0, 0.25f, 0, 1 }));

        scene.Render(framebuffer, camera, 1);*/

        /*framebuffer.Update();*/
        // --- End Frame ---

        renderer = framebuffer;
        renderer.EndFrame();
    }

    return 0;
}