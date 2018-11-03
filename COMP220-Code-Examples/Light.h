#pragma once

#include <glm\glm.hpp>

struct PointLight
{
	glm::vec4 DiffuseColour;
	glm::vec4 SpecularColour;
	glm::vec3 Position;
};
