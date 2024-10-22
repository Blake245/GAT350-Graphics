#pragma once
#include <string>
#include <SDL.h>

class Renderer
{
public:
	bool Initialize();
	bool CreateWindow(std::string title, int width, int height);

	void StartFrame();
	void EndFrame();
	
	void CopyFrameBuffer(const class Framebuffer& framebuffer);

	void operator= (const class Framebuffer& framebuffer);

	friend Framebuffer;

private:
	SDL_Renderer* m_renderer{ nullptr };
	SDL_Window* window{ nullptr };
};
