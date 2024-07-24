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
#include "vboindexer.h"
#include "controls.h"
#include "OBJLoader.h"
#include "DDSLoader.h"

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

    // 加载obj模型
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.

    // 顶点合并结果
    std::vector<unsigned short> outIndices;
    std::vector<glm::vec3> outVertices;
    std::vector<glm::vec2> outUvs;
    std::vector<glm::vec3> outNormals; // Won't be used at the moment.

    bool res = loadOBJ("suzanne.obj", vertices, uvs, normals);


    indexVBO(vertices, uvs, normals, outIndices, outVertices, outUvs, outNormals);
    // 物体原始矩阵
    glm::mat4 Model = glm::mat4(1.0f);
    // 物体的平移
    glm::vec3 moveMesh = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 meshTranslationMatrix = glm::translate(glm::mat4(1.0), moveMesh);
    // 物体的缩放
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::mat4 myScaleMatrix = glm::scale(size);
    // 物体的旋转
    glm::vec3 myRotationAxis(0.0f, 1.0f, 0.0f);
    glm::mat4 myRatateMatrix = glm::rotate(0.0f, myRotationAxis);

    // 最终矩阵
    glm::mat4 modelMatrix = meshTranslationMatrix * myRatateMatrix * myScaleMatrix;
    // This will identify our vertex buffer

    

    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    // 贴图
    // Load the texture
    GLuint Texture = loadDDS("uvmap.DDS");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // 顶点着色器数据
    GLuint modelMatrixID = glGetUniformLocation(programID, "M");
    GLuint viewMatrixID = glGetUniformLocation(programID, "V");
    GLuint projectMatrixID = glGetUniformLocation(programID, "P");

    // Give our vertices to OpenGL.
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, outVertices.size() * sizeof(glm::vec3), &outVertices[0], GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, outUvs.size() * sizeof(glm::vec2), &outUvs[0], GL_STATIC_DRAW);

    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, outNormals.size() * sizeof(glm::vec3), &outNormals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, outIndices.size() * sizeof(unsigned short), &outIndices[0], GL_STATIC_DRAW);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    glm::mat4 ViewMatrix;
    glm::mat4 projectMatrix;
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    do {
        
        // 实时获取相机矩阵
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        // 实时获取相机矩阵
       computeMatricesFromInputs(window, ViewMatrix, projectMatrix);

        // 传入矩阵
        std::cout << ViewMatrix[0][0] << std::endl;
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMatrix[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);
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

        // UV
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

        // 法线
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glVertexAttribPointer(
            2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // 索引
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        // glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDrawElements(
            GL_TRIANGLES,      // mode
            outIndices.size(),    // count
            // 根据官网的例子绘图不显示，入参类型得是GL_UNSIGNED_SHORT
            GL_UNSIGNED_SHORT,   // type
            (void*)0           // element array buffer offset
        );
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        // Swap buffers
        glfwSwapBuffers(window);

        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
        // Cleanup VBO and shader
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &uvBuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);
}