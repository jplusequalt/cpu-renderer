#pragma once

#include <vector>
#include "linalg.hpp"
#include <string>

class Model
{
public:
	Model(const char* filename);
	
	int numVerts() const;

	int numFaces() const;

	linalg::aliases::float3 vert(size_t i) const;

	std::vector<int> face(size_t i) const;

	static std::vector<std::string> split_line(std::string line);
private:
	std::vector<linalg::aliases::float3> m_verts;
	std::vector<std::vector<int>> m_faces;
};