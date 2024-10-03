#include "Renderer.h"
#include "Framebuffer.h"
#include <iostream>

bool Renderer::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Renderer::CreateWindow(std::string title, int width, int height)
{
    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // create renderer
    m_renderer = SDL_CreateRenderer(window, -1, 0);

    return true;
}

void Renderer::StartFrame()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);
}

void Renderer::EndFrame()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::CopyFrameBuffer(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::operator=(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}
