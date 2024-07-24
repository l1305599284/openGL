#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "Object3D.h"

Object3D::Object3D(glm::vec3 inPosition, glm::vec3 inRotation, glm::vec3 inScale) {
	position = inPosition;
	rotation = inRotation;
	scale = inScale;
	std::cout << "Object3D" << std::endl;
	initObject3D();
}

Object3D::~Object3D()
{
}

void Object3D::initObject3D() {
	// �����ƽ��
	glm::vec3 moveMesh = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 meshTranslationMatrix = glm::translate(glm::mat4(1.0), moveMesh);
	// ���������
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 myScaleMatrix = glm::scale(size);
	// �������ת
	glm::vec3 myRotationAxis(0.0f, 1.0f, 0.0f);
	glm::mat4 myRatateMatrix = glm::rotate(0.0f, myRotationAxis);
	// ���ؾ���
	std::cout << "���ؾ����ʼ��" << std::endl;
	matrix = meshTranslationMatrix * myRatateMatrix * myScaleMatrix;
}

void Object3D::applyMatrix4(const glm::mat4 inMatrix) {
	matrix = inMatrix;
}

void Object3D::rotateOnAxis(const glm::vec3 axis, float angle) {
	glm::mat4 rotateMatrix = glm::rotate(angle, axis);
	matrix = rotateMatrix * matrix;
}

void Object3D::setPosition(const glm::vec3 inPosition) {
	position = inPosition;
	glm::mat4 meshTranslationMatrix = glm::translate(glm::mat4(1.0), inPosition);
	// ���������
	glm::mat4 myScaleMatrix = glm::scale(scale);
	// �������ת
	glm::vec3 myRotationAxis(0.0f, 1.0f, 0.0f);
	glm::mat4 myRatateMatrix = glm::rotate(0.0f, myRotationAxis);
	// ���ؾ���
	matrix = meshTranslationMatrix * myRatateMatrix * myScaleMatrix;
}
