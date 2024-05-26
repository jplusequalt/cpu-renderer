#include "renderer.hpp"
#include "assert.h"

using linalg::aliases::float3;

Renderer::Renderer(size_t width, size_t height)
	: m_width{ width }, m_height{ height }
{
	m_framebuffer.resize(4 * width * height);
}

Renderer::Renderer(size_t width, size_t height, Renderer::Rgba color)
	: Renderer(width, height)
{
	for (size_t y = 0; y < m_height; y++)
	{
		for (size_t x = 0; x < m_width; x++)
		{
			this->setPixel(x, y, color);
		}
	}
}

void Renderer::setPixel(size_t x, size_t y, Renderer::Rgba pixel)
{
	// lodepng has origin at top left of image
	size_t yy{ m_height - y - 1 };
#ifdef DEBUG
	assert(x <= m_width && "x cannot be larger than width of image");
	assert(y <= m_height && "y cannot be larger than height of image");
#endif // DEBUG
	m_framebuffer[yy * m_width * 4 + 4 * x] = pixel.x;
	m_framebuffer[yy * m_width * 4 + 4 * x + 1] = pixel.y;
	m_framebuffer[yy * m_width * 4 + 4 * x + 2] = pixel.z;
	m_framebuffer[yy * m_width * 4 + 4 * x + 3] = pixel.w;
}

void Renderer::drawLine(int x0, int y0, int x1, int y1, Rgba color)
{
	bool steep{ false };

	// transpose the image if dy > dx
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	// swap if x0 > x1 to preserve left-right
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx{ x1 - x0 };
	int dy{ y1 - y0 };
	int dErr{ std::abs(dy) * 2 };
	int err{ 0 };
	size_t y{ static_cast<size_t>(y0) };
	int yDiff{ y1 > y0 ? 1 : -1 };

	if (steep)
	{
		for (size_t x = static_cast<size_t>(x0); x <= x1; x++)
		{
			this->setPixel(y, x, color);
			err += dErr;
			if (err > dx)
			{
				y += yDiff;
				err -= dx * 2;
			}
		}
	}
	else
	{
		for (size_t x = static_cast<size_t>(x0); x <= x1; x++)
		{
			this->setPixel(x, y, color);
			err += dErr;
			if (err > dx)
			{
				y += yDiff;
				err -= dx * 2;
			}
		}
	}
}

void Renderer::renderWireframe(const Model& m)
{
	for (size_t i = 0; i < m.numFaces(); i++)
	{
		std::vector<int> face = m.face(i);
		for (size_t j = 0; j < 3; j++)
		{
			float3 v0{ m.vert(static_cast<size_t>(face[j])) };
			float3 v1{ m.vert(static_cast<size_t>(face[(j + 1) % 3])) };

			int x0 { static_cast<int>((v0.x + 1.0) * (m_width - 1) / 2.0) };
			int y0 { static_cast<int>((v0.y + 1.0) * (m_height - 1) / 2.0) };
			int x1 { static_cast<int>((v1.x + 1.0) * (m_width - 1) / 2.0) };
			int y1 { static_cast<int>((v1.y + 1.0) * (m_height - 1) / 2.0) };

			this->drawLine(x0, y0, x1, y1, Rgba{ 255, 255, 255, 255 });
		}
	}
}

unsigned Renderer::saveImage(const char* filename)
{
	return lodepng::encode(filename, m_framebuffer, static_cast<unsigned>(m_width), static_cast<unsigned>(m_height));
}