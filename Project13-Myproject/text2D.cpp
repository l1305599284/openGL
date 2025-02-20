#include <vector>
#include <cstring>

#include <GL/glew.h>

#include "../glm/glm.hpp"//<glm/glm.hpp>
#include "../glm/gtc/matrix_transform.hpp"//<glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "DDSLoader.h"
// #include "texture.h"
#include "text2D.h"

GLuint Text2DTextureID;
GLuint Text2DVertexBufferID; //����buffer

GLuint Text2DUVBufferID;
// GLuint Text2DUVBufferID;
GLuint Text2DShaderID; // shader
GLuint Text2DUniformID;
GLuint VertexArrayID;
std::vector<glm::vec2> vertices;
std::vector<glm::vec2> UVs;

void initText2D(const char* texturePath) {
	Text2DTextureID = loadDDS(texturePath);
	// glGenVertexArrays(1, &Text2DVertexBufferID);
	// glBindVertexArray(Text2DVertexBufferID);
	// Initialize VBO
	// glGenBuffers(1, &Text2DVertexBufferID);
	// glGenBuffers(1, &Text2DUVBufferID);
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// Initialize Shader
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	Text2DShaderID = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");

	Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");
}

void drawText(const char* text, int x, int y, int size) {
	
	unsigned int length = strlen(text);
	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i < length; i++) {

		glm::vec2 vertex_up_left = glm::vec2(x + i * size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i * size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i * size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i * size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	// This will identify our vertex buffer
	// GLuint vertexbuffer;

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

	// UV
	// glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(Text2DShaderID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices GPU
	glEnableVertexAttribArray(0); // 0index gpu
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Draw the triangle !
	// glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	// glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
}