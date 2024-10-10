#pragma once
#include "Framebuffer.h"
namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer);

	void MonoChome(std::vector<color_t>& buffer);

	void Brightness(std::vector<color_t>& buffer, int brightness);

	void Noise(std::vector<color_t>& buffer, uint8_t noise);
}
