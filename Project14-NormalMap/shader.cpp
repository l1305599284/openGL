#include <string>
#include <sstream>
#include "Box.h"
#include "shader.h"

std::string getVertexShader() {
	std::string sstrVertex = "#version 330 core\nlayout(location = 0) in vec3 vertexPosition;\nuniform mat4 M;\nuniform mat4 V;\nuniform mat4 P;\nvoid main() {\ngl_Position = P* V * M * vec4(vertexPosition, 1);\n}";
	return sstrVertex;
}

std::string getFragmentShader() {
	std::string sstrFragment = "#version 330 core\nuniform vec3 inColor;\nout vec3 color;\nvoid main() {\ncolor = inColor;\n}";
	return sstrFragment;
}