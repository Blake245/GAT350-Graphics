#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Triangle.h"
#include "Sphere.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

void Model::Update()
{
	for (size_t i = 0; i < m_local_vertices.size(); i++)
	{
		m_vb[i] = m_transform * glm::vec4{ m_local_vertices[i], 1 };
	}

	// get center point of transformed vertices
	m_center = glm::vec3{ 0 };
	for (auto& vertex : m_vb)
	{
		//m_center <add vertex to center>;
		m_center += vertex;
	}
	//m_center /= <divide by the number of vertices, use float divide>;
	m_center /= (float)m_vb.size();

	// get radius of transformed vertices
	m_radius = 0;
	for (auto& vertex : m_vb)
	{
		//float radius = <use glm::length of the vector(vertex - m_center)>;
		float radius = glm::length(vertex - m_center);
		//m_radius = <use glm::max of the radius and m_radius>
		m_radius = glm::max(radius, m_radius);
	}
}

bool Model::Load(const std::string& filename)
{
	// open file using ifstream (input file stream)
	std::ifstream stream(filename);
	
	// check if stream is_open
	if (!stream.is_open())
	{
		std::cerr << "Failed to load filename" << std::endl;
		return false;
	}

	vertexbuffer_t vertices;
	std::string line;
	while (std::getline(stream, line))
	{
		// read in vertex positions
		// https://cplusplus.com/reference/string/string/substr/
		/*check for line substring == "v "*/
		if (line.substr(0, 2) == "v ")
		{
			// read position of vertex
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;

			vertices.push_back(position);
		}
		// read in faces (triangles)
		else if (line.substr(0, 2) == "f ") 
		{
			// read face (triangle), index of vertex position in vertices array [1, 2, 3]
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			// https://en.cppreference.com/w/cpp/string/basic_string/getline
			while (std::getline(sstream, str, ' '))
			{
				std::istringstream sstream(str);
				std::string indexString;

				// read each triangle index (position, normal, uv)
				size_t i = 0;
				unsigned int index[3] = { 0, 0, 0 }; // 0 = position, 1 = normal, 2 = uv
				while (std::getline(sstream, indexString, '/'))
				{
					if (!indexString.empty())
					{
						std::istringstream indexStream{ indexString };
						indexStream >> index[i];
					}
					i++;
				}

				// check if index 0 (position) is valid
				/*check if index[0] not 0*/
				if (index[0])
				{
					// get vertex at index position
					// index is 1 based, need to subtract one for array
					glm::vec3 position = vertices[index[0] - 1];

					m_local_vertices.push_back(position);
				}
			}
		}
	}

	m_vb.resize(m_local_vertices.size());

	stream.close();

	return true;
}

bool Model::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance)
{
	// check for bounding sphere raycast
	float t;
	if (!Sphere::Raycast(ray, m_center, m_radius, minDistance, maxDistance, t)) return false;

	// check cast ray with mesh triangles 
	for (size_t i = 0; i < m_vb.size(); i += 3)
	{
		//float t;
		glm::vec3 p1 = glm::vec4{ m_vb[i],	 1 };
		glm::vec3 p2 = glm::vec4{ m_vb[i + 1], 1 };
		glm::vec3 p3 = glm::vec4{ m_vb[i + 2], 1 };

		if (Triangle::Raycast(ray, p1, p2, p3, minDistance, maxDistance, t))
		{
			// set raycast hit
			raycastHit.distance = t;
			raycastHit.point = ray.At(t);

			// set edges of the triangle
			glm::vec3 edge1 = p2 - p1;
			glm::vec3 edge2 = p3 - p1;

			raycastHit.normal = glm::normalize(glm::cross(edge1, edge2));
			raycastHit.material = GetMaterial();

			return true;
		};


		/*Triangle triangle(p1, p2, p3, m_material);

		if (triangle.Hit(ray, raycastHit, minDistance, maxDistance))
		{
			return true;
		}*/
	}

	return false;
}