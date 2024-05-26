#pragma once

#include <vector>
#include <string>

#include "linalg.hpp"
#include "lodepng.hpp"
#include "model.hpp"

class Renderer
{
private:
	std::vector<unsigned char> m_framebuffer;
	size_t m_width;
	size_t m_height;
public:
	typedef linalg::vec<unsigned char, 4> Rgba;

	Renderer(size_t width, size_t height);

	Renderer(size_t width, size_t height, Rgba color);

	void setPixel(size_t x, size_t y, Rgba pixel);

	void drawLine(int x0, int y0, int x1, int y1, Rgba color);

	void renderWireframe(const Model& m);

	unsigned saveImage(const char* filename);
};