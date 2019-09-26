#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static inline void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

static inline int commonInit(GLFWwindow** outWin){
	glfwInit();
	// config glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	*outWin = window;
	if (nullptr == window) {
		std::cout << "Faild to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Faild to initialize GLAD" << std::endl;
		return -1;
	}

	// max vertex attribute 
	int maxAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
	std::cout << "Maxium num of vertex attributes supported:" << maxAttributes << std::endl;
	return 1;
}

static inline unsigned int createShaderProgram(const char* vSource, const char* fSource){
	// build and compile shader
	// --------------------------------
	// create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach shader source
	glShaderSource(vertexShader, 1, &vSource, nullptr);
	// compile shader
	glCompileShader(vertexShader);
	// check error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// create fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// build and link shaderprogram
	// --------------------------------
	// create shader program
	unsigned int shaderProgram = glCreateProgram();
	// attach shader and link program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::ATTACH_LINK_FAILED\n" << infoLog << std::endl;
	}
	// delete shader 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

static void printDes(const char* des){
	std::cout << des << std::endl;
}