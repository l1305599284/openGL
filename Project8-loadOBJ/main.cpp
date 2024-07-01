// Include standard headers
#include <stdio.h>
#include <stdlib.h>
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include <vector>
#include "shader.h"
#include "controls.h"
#include "OBJLoader.h"

GLFWwindow* window;
int main() {
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    // glfwWindowHint(GLFW_CONTEXT_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    // Open a window and create its OpenGL context

    window = glfwCreateWindow(1024, 768, "Totorial 01", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // ����objģ��
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res = loadOBJ("cube.obj", vertices, uvs, normals);

    // ����ԭʼ����
    glm::mat4 Model = glm::mat4(1.0f);
    // �����ƽ��
    glm::vec3 moveMesh = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 meshTranslationMatrix = glm::translate(glm::mat4(1.0), moveMesh);
    // ���������
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::mat4 myScaleMatrix = glm::scale(size);
    // �������ת
    glm::vec3 myRotationAxis(0.0f, 1.0f, 0.0f);
    glm::mat4 myRatateMatrix = glm::rotate(0.0f, myRotationAxis);

    // ���վ���
    glm::mat4 transformeMatrix = meshTranslationMatrix * myRatateMatrix * myScaleMatrix;
    // This will identify our vertex buffer

    // �������
    glm::vec3 cameraPosition = glm::vec3(-5.0f, 5.0f, 0.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 CameraMatrix = glm::lookAt(
        cameraPosition, // the position of your camera, in world space
        cameraTarget,   // where you want to look at, in world space
        upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
    );



    // ͶӰ����
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90&deg; (extra wide) and 30&deg; (quite zoomed in)
        4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );




    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // ��ͼ
    // loadTGA_custom("uvtemplate.tga");
    // loadBMP_custom("uvtemplate.bmp");

    // Give our vertices to OpenGL.
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    do {
        // ʵʱ��ȡ�������
        glm::mat4 VPMatrix = computeMatricesFromInputs(window);

        glm::mat4 MVPmatrix = VPMatrix * transformeMatrix;// transformeMatrix;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        // �������
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPmatrix[0][0]);
        // glClear(GL_COLOR_BUFFER_BIT);
        // Draw nothing, see you in tutorial 2 !
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        // ���
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        /*
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        */
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(glm::vec3)); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        // Swap buffers
        glfwSwapBuffers(window);

        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}