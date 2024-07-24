#pragma once
#include "../glm/ext/matrix_float4x4.hpp"
#include <string>
class Material
{
	private:
		std::string type = "Material";
	public:
		glm::vec3 color = glm::vec3(1.0,0.0,0.0);
		Material(glm::vec3 inColor = glm::vec3(1.0, 1.0, 1.0));
};

