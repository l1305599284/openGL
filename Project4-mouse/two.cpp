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
#include "shader.hpp"
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
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };
    // 物体原始矩阵
    glm::mat4 Model = glm::mat4(1.0f);
    // 物体的平移
    glm::vec3 moveMesh = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 meshTranslationMatrix = glm::translate(glm::mat4(1.0), moveMesh);
    // 物体的缩放
    glm::vec3 size = glm::vec3(3.0f, 3.0f, 3.0f);
    glm::mat4 myScaleMatrix = glm::scale(size);
    // 物体的旋转
    glm::vec3 myRotationAxis(0.0f , 1.0f, 0.0f);
    glm::mat4 myRatateMatrix = glm::rotate(0.0f, myRotationAxis);

    // 最终矩阵
    glm::mat4 transformeMatrix = meshTranslationMatrix * myRatateMatrix * myScaleMatrix;
    // This will identify our vertex buffer

    // 相机矩阵
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 CameraMatrix = glm::lookAt(
        cameraPosition, // the position of your camera, in world space
        cameraTarget,   // where you want to look at, in world space
        upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
    );

    // 投影矩阵
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90&deg; (extra wide) and 30&deg; (quite zoomed in)
        4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );
    
    glm::mat4 MVPmatrix = projectionMatrix * CameraMatrix * transformeMatrix;// transformeMatrix;

    
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Give our vertices to OpenGL.
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        // 传入矩阵
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
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        // Swap buffers
        glfwSwapBuffers(window);
        
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}