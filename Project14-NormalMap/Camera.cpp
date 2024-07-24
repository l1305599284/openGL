#include "Camera.h"
Camera::Camera(float inFov, float inAspect, float inNear, float inFar) {
	fov = inFov;
	aspect = inAspect;
	near = inNear;
	far = inFar;
	std::cout << "Camera" << std::endl;
	viewMatrix = glm::lookAt(this->position, this->target, this->up);
	projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
};

void Camera::lookAt(const glm::vec3 inTarget) {
	this->target = inTarget;
	viewMatrix = glm::lookAt(this->position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
	std::cout << "CameraLookAt" << std::endl;
	std::cout << viewMatrix[0][0] << std::endl;
	std::cout << viewMatrix[0][1] << std::endl;
	std::cout << viewMatrix[0][2] << std::endl;
	std::cout << viewMatrix[0][3] << std::endl;
	std::cout << "CameraLookAt" << std::endl;
	std::cout << "CameraLookAt" << std::endl;
	std::cout << projectionMatrix[0][0] << std::endl;
	std::cout << projectionMatrix[0][1] << std::endl;
	std::cout << projectionMatrix[0][2] << std::endl;
	std::cout << projectionMatrix[0][3] << std::endl;
	std::cout << "CameraLookAt" << std::endl;
}