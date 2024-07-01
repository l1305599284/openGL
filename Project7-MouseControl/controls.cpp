#include <GL/glew.h>
// Include GLFW
#include <iostream>
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"
// #include "../glm/gtx/transform.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "controls.h"

// ��ʼ�����position
glm::vec3 position = glm::vec3(0, 0, 5);
// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 VPMatrix;
float speed = 0.03f; // 3 units / second
float mouseSpeed = 0.005f;
float lastTime = 0.0f;
float getScrollFov = 0.0f;
double FoV = 45.0f;

// ��껬��
void scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset)
{
    FoV = FoV - 5 * yoffset;
    std::cout << "Value is " << FoV << std::endl;
    // FoV -= (float)yoffset;
    // std::cout << "Value is " << FoV << std::endl;
    if (FoV < 20.0f)
        FoV = 20.0f;
    if (FoV > 100.0f)
        FoV = 100.0f;
}

glm::mat4 computeMatricesFromInputs(GLFWwindow* window) {
    // ��ȡʱ����
    
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    lastTime = deltaTime;
    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024.0 / 2, 768.0 / 2);
    // Compute new orientation
    horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
    verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);

    // �����������
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // �����������
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // �����������Up vector : perpendicular to both direction and right
    glm::vec3 up = glm::cross(right, direction);
    // Move forward�������Ҽ�
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }
    glfwSetScrollCallback(window, scroll_callback_static);
    // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit  100 units
    ProjectionMatrix = glm::perspective(glm::radians(float(FoV)), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
        position,           // Camera is here
        position + direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );
    VPMatrix = ProjectionMatrix * ViewMatrix;
    return VPMatrix;
};

