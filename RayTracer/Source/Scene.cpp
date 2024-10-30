#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Tracer.h"
#include "Random.h"
#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples)
{
	for (int y = 0; y < framebuffer.m_height; y++)
	{
		for (int x = 0; x < framebuffer.m_width; x++)
		{
			color3_t color{ 0 };
			for (int i = 0; i < numSamples; i++)
			{
				glm::vec2 pixel{ x, y };
				pixel += glm::vec2{ randomf(), randomf() };
				glm::vec2 point = pixel / glm::vec2{ framebuffer.m_width, framebuffer.m_height };
				point.y = 1 - point.y;

				ray_t ray = camera.GetRay(point);

				color = Tracer::Trace(*this, ray, 0.001f, 100.0f);
				//color4_t color = { 0.5f, 0, 0, 1 };

			}
			// average color
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
		std::cout << "y: " << y << std::endl;
	}
}
