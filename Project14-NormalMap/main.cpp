// Include standard headers
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Material.h"
#include "Scene.h"
#include "Camera.h"
#include "Box.h"
#include "Renderer.h"


int main() {
    Camera myCamera = Camera(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
    myCamera.setPosition(glm::vec3(10.0, 10.0, 10.0));
    myCamera.lookAt(glm::vec3(0.0, 0.0, 0.0));

    Scene myScene = Scene();
    Material redMaterial = Material(glm::vec3(1.0, 0.0, 0.0));
    // 初始创建BOX
    Box myBox = Box(redMaterial);

    Material greenMaterial = Material(glm::vec3(0.0, 1.0, 0.0));
    Box myBox2 = Box(greenMaterial);
    myBox2.setPosition(glm::vec3(5.0, 0.0, 0.0));
    // 修改了位置
    // myBox.setPosition(glm::vec3(10.0, 10.0, 10.0));
    myScene.add(myBox);
    myScene.add(myBox2);
    Renderer myRenderer = Renderer();
    myRenderer.render(myScene, myCamera);
}