#pragma once
#include "Object3D.h"

class Camera : public Object3D
{
	public:
		float fov;
		float aspect;
		float near;
		float far;
		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		Camera(float inFov, float inAspect, float inNear, float inFar);
		void lookAt(const glm::vec3 target);
	private:
		std::string type = "Camera";
		
};
