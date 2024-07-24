#pragma once
#include "Object3D.h"
#include "Material.h"
#include <vector>
#include <GL/glew.h>
class Box : public Object3D
{
	public:
		std::vector<glm::vec3> getVertex();
		Material material;
		Box(Material inMaterial);
	private:
		std::string type = "Box";
        GLfloat* vertices = new GLfloat[108];
};

