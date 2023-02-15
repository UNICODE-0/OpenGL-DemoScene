#include "OpenGLDebug.hpp"

void printShaderLog(GLuint shader)
{
	int length = 0;
	int charWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	if(length > 0)
	{
		infoLog = (char*)malloc(length);
		glGetShaderInfoLog(shader, length, &charWritten, infoLog);
		std::cout << infoLog;
		free(infoLog);
	}
}
void printProgramLog(int program)
{
	int length = 0;
	int charWritten = 0;
	char *infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	if(length > 0)
	{
		infoLog = (char*)malloc(length);
		glGetProgramInfoLog(program, length, &charWritten, infoLog);
		std::cout << infoLog << std::endl;
		free(infoLog);
	}
}
bool checkOpenGLError()
{
	bool foundError = false;
	int GlError = glGetError();
	
	while(GlError != GL_NO_ERROR)
	{
		std::cout<< getFormattedString("glError: ", StringColor::Red, StringType::Bold) << GlError << std::endl;
		foundError = true;
		int GlError = glGetError();
	}
	
	return foundError;
}