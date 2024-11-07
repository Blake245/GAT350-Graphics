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
#include "Triangle.h"
#include "Model.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

void InitScene(Scene& scene);
void CornellBoxScene(Scene& scene);

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
    camera.SetView({ 0, 2, -17}, { 0,0,0 });

    Scene scene;
    //InitScene(scene);
    CornellBoxScene(scene);

    
    //framebuffer.Clear(ColorConvert(color4_t{ 0, 0.25f, 0, 1 }));
    scene.Update();
    scene.Render(framebuffer, camera, 200, 6);
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

void InitScene(Scene& scene)
{
    //scene.SetSky(HSVtoRGB(240, 1, 0.5f), HSVtoRGB(240, 1, 1));


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
    std::shared_ptr<Material> yellowLight = std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 10.0f);

    std::array<std::shared_ptr<Material>, 12> materials = { red, blue, green, purple,
        redMetal, blueMetal, greenMetal, purpleMetal,
        clear, redLight, blueLight, yellowLight };

    int matnum1 = random(9);
    //auto sphere = std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 7, 0 } }, 5.0f, materials[matnum1]);
    //auto sphere = std::make_unique<Sphere>(glm::vec3{ 5, 20, 0 }, 5.0f, clear);
    //scene.AddObject(std::move(sphere));

    //auto triangle = std::make_unique<Triangle>(glm::vec3{ -2, 0, 0 }, glm::vec3{ 0, 2, 0 }, glm::vec3{ 2, 0, 0 }, red); 
    ////auto triangle = std::make_unique<Triangle>(glm::vec3{ 0, 5, 0 }, glm::vec3{ 5, 5, 0 }, glm::vec3{ 0, 0, 0 }, redMetal); 
    //scene.AddObject(std::move(triangle));


    //std::shared_ptr<Material> material2 = std::make_shared<Material>(color3_t{ 0, 0, 1 });
    auto plane = std::make_unique<Plane>(Transform{ glm::vec3{0, -2, 0} }, gray);
    scene.AddObject(std::move(plane));

    /*for (int i = 0; i < 10; i++)
    {
        int matnum = random(12);
        auto object = std::make_unique<Sphere>(Transform{ glm::vec3{ random(glm::vec3{ -10.0f, 0, -10.0f }, glm::vec3{ 10.0f })} }, randomf(1.0f, 3.0f), materials[random(12)]);
        scene.AddObject(std::move(object));
    }*/

    //auto model = std::make_unique<Model>(materials[random(12);
    auto model = std::make_unique<Model>(Transform{ glm::vec3{0, 2, 2}, glm::vec3{20, 40, 0}, glm::vec3{1} }, std::make_shared<Lambertian>(HSVtoRGB(240.0f, 1.0f, 1.0f)));
    model->Load("../Build/Model/cube.obj");
    scene.AddObject(std::move(model));
}

void CornellBoxScene(Scene& scene)
{
    scene.SetSky(color3_t{ 0 }, color3_t{ 0.5f });

    //Materials
    std::shared_ptr<Material> white = std::make_shared<Lambertian>(color3_t{ 1.0f });
    std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    std::shared_ptr<Material> green = std::make_shared<Lambertian>(color3_t{ 0, 1, 0 });
    std::shared_ptr<Material> whiteLight = std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 3);

    // Walls
    auto floor = std::make_unique<Plane>(Transform{ glm::vec3{0, -5, 0} }, white);
    scene.AddObject(std::move(floor));
    
    auto ceiling = std::make_unique<Plane>(Transform{ glm::vec3{0, 10, 0} }, white);
    scene.AddObject(std::move(ceiling));

    auto lWall = std::make_unique<Plane>(Transform{ glm::vec3{-8, 0, 0}, glm::vec3{0, -5, 90} }, red);
    scene.AddObject(std::move(lWall));
    
    auto rWall = std::make_unique<Plane>(Transform{ glm::vec3{8, 0, 0}, glm::vec3{0, 5, -90} }, green);
    scene.AddObject(std::move(rWall));
    
    auto bWall = std::make_unique<Plane>(Transform{ glm::vec3{0, 0, 10}, glm::vec3{90, 0, 0} }, white);
    scene.AddObject(std::move(bWall));

    // lights
    auto light = std::make_unique<Model>(Transform{ glm::vec3{0, 10.2f, 0}, glm::vec3{0, 0, 0}, glm::vec3{3, 0.5f, 3} }, whiteLight);
    light->Load("../Build/Model/cube.obj");
    scene.AddObject(std::move(light));

    // objects
    auto sphere = std::make_unique<Sphere>(Transform{ glm::vec3{ 3, -2, -5 } }, 3.0f, white);
    scene.AddObject(std::move(sphere));

    auto cube = std::make_unique<Model>(Transform{ glm::vec3{-3, -2.5f, 0}, glm::vec3{0, 40, 0}, glm::vec3{5} }, white);
    cube->Load("../Build/Model/cube.obj");
    scene.AddObject(std::move(cube));
}