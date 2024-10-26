#include "Framebuffer.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "Image.h"
#include <iostream>

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);

	if (!m_texture)
	{
		std::cerr << "Error Creating Texture: " << SDL_GetError() << std::endl;
	}
	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	color_t& dest = m_buffer[x + (y * m_width)];
	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawPointClip(int x, int y, const color_t& color)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;

	color_t& dest = m_buffer[x + (y * m_width)];
	dest = AlphaBlend(color, dest);
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	if (x + w < 0 || x >= m_width || y + h < 0 || y >= m_height) return;

	int x1 = std::max(x, 0);
	int x2 = std::min(x + w, m_width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + h, m_height);

	//LineClip(x1, x2, y1, y2);

	for (int sy = y1; sy < y2; sy++)
	{
		for (int sx = x1; sx < x2; sx++)
		{
			//m_buffer[sx + (sy * m_width)] = color;
			DrawPoint(sx, sy, color);
		}
	}
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;  //run 
	int dy = y2 - y1;  //rise

	if (dx == 0)
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y < y2; y++)
		{
			m_buffer[x1 + y * m_width] = color;
		}
	}
	else
	{
		// y = mx + b (slope intercept)
		// m = rise / run
		float m = dy / (float)dx;
		// b = y - (m * x)
		float b = y1 - (m * x1);

		// draw line points
		if (std::abs(dx) > std::abs(dy))
		{
			for (int x = x1; x <= x2; x++)
			{
				int y = (int)round((m * x) + b);
				m_buffer[x + y * m_width] = color;
			}
		}
		else
		{
			for (int y = y1; y <= y2; y++)
			{
				// x = (y - b) / m
				int x = (int)round((y - b) / m);
				m_buffer[x + y * m_width] = color;
			}
		}
	}
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	if (!LineClip(x1, x2, y1, y2)) return;

	// calculate deltas
	int dx = x2 - x1; 
	int dy = y2 - y1;  

	// check for steep line
	bool steep = (std::abs(dy) > std::abs(dx));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	// ensure left to right drawing
	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	// recalculate deltas
	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	// set error term and y step direction
	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	// draw line points
	for (int x = x1, y = y1; x <= x2; x++)
	{
		//LineClip(x1, x2, y1, y2);
		// error here
		(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);

		// update error term
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);

}

void Framebuffer::DrawCircle(int xc, int yc, int radius, const color_t& color)
{
	 //Bresenham's algorithm
	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;
	DrawOctants(xc, yc, x, y, color);
	 //check for decision parameter and update d, y
	while (y >= x)
	{
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
		{
			d = d + 4 * x + 6;
		}
			//Increment x after updating decision parameter
			x++;

			//Draw the circle with new coordinates
			DrawOctants(xc, yc, x, y, color);
			
	}
}

void Framebuffer::DrawOctants(int xc, int yc, int x, int y, const color_t& color)
{
	DrawPoint(xc + x, yc + y, color);
	DrawPoint(xc - x, yc + y, color);
	DrawPoint(xc + x, yc - y, color);
	DrawPoint(xc - x, yc - y, color);
	DrawPoint(xc + y, yc + x, color);
	DrawPoint(xc - y, yc + x, color);
	DrawPoint(xc + y, yc - x, color);
	DrawPoint(xc - y, yc - x, color);
}

//enum Region {
//};
	const int INSIDE = 0; // 0000
	const int LEFT = 1; // 0001
	const int RIGHT = 2; // 0010
	const int BOTTOM = 4; // 0100
	const int TOP = 8; // 1000

int Framebuffer::SetRegionCode(int x, int y)
{
	int code = INSIDE;

	if (x < 0)                code |= LEFT;
	else if (x >= m_width)    code |= RIGHT;
	if (y < 0)                code |= BOTTOM;
	else if (y >= m_height)   code |= TOP;

	return code;
}

bool Framebuffer::LineClip(int& x1, int& x2, int& y1, int& y2)
{
	// Compute region codes for P1, P2
	int code1 = SetRegionCode(x1, y1);
	int code2 = SetRegionCode(x2, y2);

	bool accept = false;

	while (true)
	{
		if ((code1 == 0) && (code2 == 0))
		{
			// Both endpoints lie inside the rectangle
			accept = true;
			break;
		}
		else if (code1 & code2)
		{
			// Both endpoints are outside the rectangle in the same region
			break;
		}
		else
		{
			// Some segment of the line lies within the rectangle
			int code_out;
			int x = 0, y = 0;

			// At least one endpoint is outside the rectangle, pick it
			if (code1 != 0)
				code_out = code1;
			else
				code_out = code2;

			// Find intersection point using formulas:
			// y = y1 + slope * (x - x1)
			// x = x1 + (1 / slope) * (y - y1)
			if (code_out & TOP)
			{
				// Point is above the rectangle

				x = x1 + (x2 - x1) * (m_height - y1) / (y2 - y1);
				y = m_height - 1;
				//std::cout << "Top" << std::endl;
			}
			else if (code_out & BOTTOM)
			{
				// Point is below the rectangle
				x = x1 + (x2 - x1) * (0 - y1) / (y2 - y1);
				y = 0;
				//std::cout << "Bottom" << std::endl;
			}
			else if (code_out & RIGHT)
			{
				// Point is to the right of rectangle
				y = y1 + (y2 - y1) * (m_width - x1) / (x2 - x1);
				x = m_width - 1;
				//std::cout << "right" << std::endl;
			}
			else if (code_out & LEFT)
			{
				// Point is to the left of rectangle
				y = y1 + (y2 - y1) * (0 - x1) / (x2 - x1);
				x = 0;
				//std::cout << "left" << std::endl;
			}

			// Replace the outside point with the intersection point
			// and get the new region code
			if (code_out == code1)
			{
				x1 = x;
				y1 = y;
				code1 = SetRegionCode(x1, y1);
			}
			else
			{
				x2 = x;
				y2 = y;
				code2 = SetRegionCode(x2, y2);
			}
		}
	}

	return accept;
}

void Framebuffer::DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color)
{
	float dt = 1.0f / 10;
	float t1 = 0;
	for(int i = 0; i < 10; i++)
	{ 
		int sx1 = Lerp(x1, x2, t1);
		int sy1 = Lerp(y1, y2, t1);

		float t2 = t1 + dt;

		int sx2 = Lerp(x1, x2, t2);
		int sy2 = Lerp(y1, y2, t2);

		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}


}

void Framebuffer::DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	float dt = 1.0f / 100;
	float t1 = 0;
	for (int i = 0; i < 100; i++)
	{
		int sx1, sy1;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t1, sx1, sy1);
		float t2 = t1 + dt;

		int sx2, sy2;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t2, sx2, sy2);

		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawCubicCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const color_t& color)
{
	float dt = 1.0f / 100;
	float t1 = 0;
	for (int i = 0; i < 100; i++)
	{
		int sx1, sy1;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t1, sx1, sy1);
		
		float t2 = t1 + dt;
		int sx2, sy2;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t2, sx2, sy2);

		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawImage(int x, int y, const Image& image)
{
	// check if off-screen
	if (x + image.m_width < 0 || x >= m_width || y + image.m_height < 0 || y >= m_height) return;

	// iterate through image y
	for (int iy = 0; iy < image.m_height; iy++)
	{
		// set screen y 
		int sy = y + iy;
		// check if off-screen, don't draw if off-screen
		if (sy < 0 || sy >= m_height) continue;

		// iterate through image x
		for (int ix = 0; ix < image.m_width; ix++)
		{
			// set screen x
			int sx = x + ix;
			// check if off-screen, don't draw if off-screen
			if (sx < 0 || sx >= m_width) continue;

			// get image pixel color
			color_t color = image.m_buffer[ix + (iy * image.m_width)];
			
			// set buffer to color
			DrawPoint(sx, sy, color);

			//m_buffer[sx + (sy * m_width)] = color;
		}
	}
}



