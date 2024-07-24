#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/ext/matrix_float4x4.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"
#include <iostream>
#include <string>
class Object3D
{
	public:
		std::string name;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
		glm::mat4 matrix = glm::mat4(1.0f);
		void initObject3D();
		void applyMatrix4(const glm::mat4 inMatrix);
		void rotateOnAxis(const glm::vec3 axis, float angle);
		void setPosition(const glm::vec3 inPosition);
		Object3D(glm::vec3 inPosition = glm::vec3(0.0), glm::vec3 inRotation = glm::vec3(0.0), glm::vec3 inScale = glm::vec3(1.0));
		~Object3D();
	private:
		std::string type = "Object3D";
};

