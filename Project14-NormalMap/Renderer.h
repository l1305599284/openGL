#pragma once
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include "Scene.h"
#include "Camera.h"

class Renderer {
	public:
		GLchar mode;
		int start;
		int count;
		void render(Scene inScene, Camera inCamera);
	private:
		GLuint makeShader(std::string VertexShaderCode, std::string FragmentShaderCode);
		GLuint getBuffer(std::vector<glm::vec3> buffer_data);
		void setMatrixLocation(GLuint programID, const GLchar* locationName, glm::mat4 matrix);
		void setVectorLocation(GLuint programID, const GLchar* locationName, GLfloat* vector);
		void BindBuffer(int index, GLuint buffer);
		void draw(int index);
		void clear();
		int initOpenGLWindow(int windowWidth, int windowHeight);
};

