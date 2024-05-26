#include "model.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using linalg::aliases::float3;

Model::Model(const char* filename)
	: m_verts{}, m_faces{}
{
	std::ifstream objFile{ filename };

	if (!objFile)
	{
		std::cerr << "Failed to open file: " << filename << '\n';
		return;
	}

	std::string line{};
	while (std::getline(objFile, line))
	{
		if (!line.compare(0, 2, "v "))
		{
			auto split{ split_line(line) };
			m_verts.push_back(float3{ static_cast<float>(atof(split[1].c_str())),
				static_cast<float>(atof(split[2].c_str())),
				static_cast<float>(atof(split[3].c_str())) });
		}
		else if (!line.compare(0, 2, "f "))
		{
			auto split{ split_line(line) };
			std::vector faces{ atoi(split_line(split[1])[0].c_str()) - 1 };
			faces.push_back(atoi(split_line(split[2])[0].c_str()) - 1);
			faces.push_back(atoi(split_line(split[3])[0].c_str()) - 1);

			m_faces.push_back(faces);
		}
	}
}

int Model::numVerts() const
{
	return static_cast<int>(m_verts.size());
}

int Model::numFaces() const
{
	return static_cast<int>(m_faces.size());
}

float3 Model::vert(size_t i) const
{
	return m_verts[i];
}

std::vector<int> Model::face(size_t i) const
{
	return m_faces[i];
}

std::vector<std::string> Model::split_line(std::string line)
{
	std::stringstream ss(line);
	std::vector<std::string> result;
	std::string token;

	while (std::getline(ss, token, ' ')) {
		result.push_back(token);
	}

	return result;
}
