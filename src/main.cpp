// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Self-written
#include "../lib/FileSystemUtilities.hpp"
#include "../lib/StringUtilities.hpp"
#include "../lib/ShaderUtilities.hpp"
#include "../lib/3DModelUtilities.hpp"
#include "../lib/OpenGLDebug.hpp"

// Other
#include <iostream>
#include <cmath>
#include <fstream>
#include <stack>

#define numVAOs 1
#define numVBOs 4

#define FOV 1.0472f // = 60 degrees
#define farPlane 1000.0f
#define nearPlane 0.1f

WavefrontModel wavefrontModel("obj/Sculpture.obj");

int frameBufferWidth, frameBufferHeight;
float aspect;
float cameraLocX, cameraLocY, cameraLocZ;
float objLocX, objLocY, objLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint globalAmbientLoc;
GLuint lightAmbientLoc, lightDiffuseLoc, lightSpecularLoc, lightPositionLoc;
GLuint matAmbientLoc, matDiffuseLoc, matSpecularLoc, matShininessLoc;

GLuint mvLoc, projLoc, inverseLoc, timeLoc;
GLuint brickTexture;
GLuint sculptureTexture;

glm::mat4 pMat, vMat, mMat, mvMat, inverseMV;
glm::vec3 lightPos, lightViewPos;
float lightViewPosValues[3];

float globalAmbient[4] = {0.7f, 0.7f, 0.7f, 1.0f};
float lightAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float lightSpecular[4] = {0.7f, 0.7f, 0.7f, 1.0f};

float ma[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
float* matAmbient = ma;
float md[4] = { 0.8f, 0.8f, 0.8f, 0.8f };
float* matDiffuse = md;
float ms[4] = { 0.08f, 0.08f, 0.05f, 1.0f };
float* matSpecular = ms; 
float matShininess = 52.2f; 

void setupVertices()
{
	std::vector<float> vertices = wavefrontModel.getRawVertices();
	std::vector<float> textureCoords = wavefrontModel.getRawTextureCoords();
	std::vector<float> normals = wavefrontModel.getRawNormals();

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(3, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * 4, &textureCoords[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, &normals[0], GL_STATIC_DRAW);
}

void setupLights(glm::mat4 vMatrix)
{
	lightViewPos = glm::vec3(vMatrix * glm::vec4(lightPos, 1.0f));
	lightViewPosValues[0] = lightViewPos.x;
	lightViewPosValues[1] = lightViewPos.y;
	lightViewPosValues[2] = lightViewPos.z;

	globalAmbientLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	lightAmbientLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	lightDiffuseLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	lightSpecularLoc = glGetUniformLocation(renderingProgram, "light.specular");
	lightPositionLoc = glGetUniformLocation(renderingProgram, "light.position");
	matAmbientLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	matDiffuseLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	matSpecularLoc = glGetUniformLocation(renderingProgram, "material.specular");
	matShininessLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	glProgramUniform4fv(renderingProgram, globalAmbientLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, lightAmbientLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, lightDiffuseLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, lightSpecularLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, lightPositionLoc, 1, lightViewPosValues);
	glProgramUniform4fv(renderingProgram, matAmbientLoc, 1, matAmbient);
	glProgramUniform4fv(renderingProgram, matDiffuseLoc, 1, matDiffuse);
	glProgramUniform4fv(renderingProgram, matSpecularLoc, 1, matSpecular);
	glProgramUniform1f(renderingProgram, matShininessLoc, matShininess);
}

void window_reshape_callback(GLFWwindow* window, int width, int height)
{
	aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat = glm::perspective(FOV, aspect, nearPlane, farPlane);
}

GLuint createShaderProgram()
{
	setIncludeFromFile("TransformationMatrices.glsl", "shaders/lib");

	std::string vertexShaderString = readFromFile("shaders/VertexShader.glsl");
	std::string fragmentShaderString = readFromFile("shaders/FragmentShader.glsl");

	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragmentShaderSource = fragmentShaderString.c_str();
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(vertexShader);
	checkOpenGLError();
	int shaderCompileStatus = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompileStatus);
	if(shaderCompileStatus != 1)
	{
		std::cout << getFormattedString("Invalid vertex shader", StringColor::Red, StringType::Bold) << std::endl;
		printShaderLog(vertexShader);
	}

	glCompileShader(fragmentShader);
	checkOpenGLError();
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompileStatus);
	if(shaderCompileStatus != 1)
	{
		std::cout << getFormattedString("Invalid fragment shader", StringColor::Red, StringType::Bold) << std::endl;
		printShaderLog(fragmentShader);
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	checkOpenGLError();
	int programLinkingStatus = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &programLinkingStatus);
	if(programLinkingStatus != 1)
	{
		std::cout<< getFormattedString("linking failed", StringColor::Red, StringType::Bold) << std::endl;
		printProgramLog(program);
	}
	
	return program;
}

void init(GLFWwindow* window)
{	
	renderingProgram = createShaderProgram();

	cameraLocX = 0.0f; cameraLocY = 3.8f; cameraLocZ = 6.0f;
	objLocX = 0.0f; objLocY = 2.0f; objLocZ = 1.8f;
	lightPos = glm::vec3(0.4f, 4.0f, 3.4f);
	sculptureTexture = loadTexture("img/SculptureTexture.jpg");

	setupVertices();
	
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	aspect = (float)frameBufferWidth / (float)frameBufferHeight;
	pMat = glm::perspective(FOV, aspect, nearPlane, farPlane);
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mvMatrix");
	projLoc = glGetUniformLocation(renderingProgram, "projMatrix");
	timeLoc = glGetUniformLocation(renderingProgram, "time");
	inverseLoc = glGetUniformLocation(renderingProgram, "inverseMV");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLocX, -cameraLocY, -cameraLocZ));

	setupLights(vMat);

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));
	mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0, 1, 0));
	mvMat = vMat * mMat;

	inverseMV = glm::transpose(glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(inverseLoc, 1, GL_FALSE, glm::value_ptr(inverseMV));
	glProgramUniform1f(renderingProgram, timeLoc, (float)currentTime);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sculptureTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, wavefrontModel.getNumVertices());
}

int main(void)
{
	if(!glfwInit()) {exit(EXIT_FAILURE);}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1670, 1373, "OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	if(glewInit() != GLEW_OK){exit(EXIT_FAILURE);}
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_reshape_callback);

	init(window);

	while(!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}