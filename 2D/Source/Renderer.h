#pragma once
#include <string>
#include <SDL.h>

class Renderer
{
public:
	bool Initialize();
	bool CreateWindow(std::string title, int width, int height);

	void ClearFrame();
	void PresentFrame();
private:
	SDL_Renderer* m_renderer{ nullptr };
	SDL_Window* window{ nullptr };
};
