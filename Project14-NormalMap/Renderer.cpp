#include "Renderer.h"
#include "shader.h"
GLFWwindow* window;

void Renderer::render(Scene inScene, Camera inCamera)
{
	initOpenGLWindow(1024, 768);
	std::cout << "开始渲染" << std::endl;

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	int childrenSize = inScene.children.size();
	std::cout << childrenSize << std::endl;
	std::string vertexCode = getVertexShader();
	std::string fragmentCode = getFragmentShader();
	GLuint programID = Renderer::makeShader(vertexCode, fragmentCode);

	GLuint MatrixID1 = glGetUniformLocation(programID, "M");
	GLuint MatrixID2 = glGetUniformLocation(programID, "V");
	GLuint MatrixID3 = glGetUniformLocation(programID, "P");
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
	
		setMatrixLocation(programID, "P", inCamera.projectionMatrix);
		setMatrixLocation(programID, "V", inCamera.viewMatrix);
		setMatrixLocation(programID, "M", inScene.children[0].matrix);
		std::cout << "do" << std::endl;
		
		for (int i = 0; i < childrenSize; i++) {
			// 此处组装着色器
			setMatrixLocation(programID, "M", inScene.children[i].matrix);
			GLuint boxVertexBuffer = getBuffer(inScene.children[i].getVertex());
			GLfloat color_data[] = { inScene.children[i].material.color.x, inScene.children[i].material.color.y, inScene.children[i].material.color.z };
			setVectorLocation(programID, "inColor", color_data);
			BindBuffer(0, boxVertexBuffer);
			draw(i);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	clear();
}



GLuint Renderer::getBuffer(std::vector<glm::vec3> buffer_data) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, buffer_data.size() * sizeof(glm::vec3), &buffer_data[0], GL_STATIC_DRAW);
	return buffer;
}

void Renderer::setMatrixLocation(GLuint programID, const GLchar* locationName, glm::mat4 matrix) {
	GLuint locationID = glGetUniformLocation(programID, locationName);
	glUniformMatrix4fv(locationID, 1, GL_FALSE, &matrix[0][0]);
}

void Renderer::setVectorLocation(GLuint programID, const GLchar* locationName, GLfloat* vector) {
	GLuint locationID = glGetUniformLocation(programID, locationName);
	glUniform3fv(programID, 1, vector);
}

void Renderer::BindBuffer(int index, GLuint buffer) {
	glEnableVertexAttribArray(index);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		index,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
}

void Renderer::draw(int index) {
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(index);
}


GLuint Renderer::makeShader(std::string VertexShaderCode, std::string FragmentShaderCode) {
	GLint Result = GL_FALSE;
	int InfoLogLength;
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Compile Vertex Shader
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	std::cout << "构建成功" << std::endl;
	return ProgramID;
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::cout << "Clear" << std::endl;
}
 

int Renderer::initOpenGLWindow(int windowWidth, int windowHeight) {
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(windowWidth, windowHeight, "Totorial 01", NULL, NULL);
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	// glEnable(GL_CULL_FACE);
	// 启用混合
	// glEnable(GL_BLEND);
	// 设置混合因子
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	return 0;
}