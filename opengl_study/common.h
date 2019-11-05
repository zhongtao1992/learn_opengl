#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
// glm
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>
#include <functional>

static float g_viewWidth = 800.0f;
static float g_viewHeight = 600.0f;

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
	GLFWwindow *window = glfwCreateWindow(g_viewWidth, g_viewHeight, "LearnOpenGL", nullptr, nullptr);
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

static inline int commonInitWithMultiSample(GLFWwindow** outWin, const int& sampleCount){
	glfwInit();
	// config glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// enable multisample
	glfwWindowHint(GLFW_SAMPLES, sampleCount);

	// create window
	GLFWwindow *window = glfwCreateWindow(g_viewWidth, g_viewHeight, "LearnOpenGL", nullptr, nullptr);
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

// var
static glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static float g_lastX = g_viewWidth * 0.5f;
static float g_lastY = g_viewHeight * 0.5f;
static float g_fov = 45.0f;
// if yaw value is 0.0f, once we first move mouse, the camera will make a large jume to left.
static float g_yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
static float g_pitch = 0.0f;
static bool g_bFirstMouse = true;
// var
static std::vector<std::function<void(GLFWwindow* window, double xpos, double ypos)>> g_mouseCallBack;
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (g_bFirstMouse) {
		g_bFirstMouse = false;
		g_lastX = xpos;
		g_lastY = ypos;
	}
	float xoffset = xpos - g_lastX;
	float yoffset = g_lastY - ypos; // revert
	g_lastX = xpos;
	g_lastY = ypos;
	float sensitivity = 0.08f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	g_pitch += yoffset;
	g_yaw += xoffset;

	// constraint
	if (g_pitch > 89.0f) {
		g_pitch = 89.0f;
	}
	if (g_pitch < -89.0f) {
		g_pitch = -89.0f;
	}
	float _pitch = glm::radians(g_pitch);
	float _yaw = glm::radians(g_yaw);
	// calcute direction
	glm::vec3 front;
	front.x = cos(_yaw) * cos(_pitch);
	front.y = sin(_pitch);
	front.z = sin(_yaw) * cos(_pitch);
	g_cameraFront = glm::normalize(front);

	for (auto v : g_mouseCallBack){
		v(window, xpos, ypos);
	}
}

static std::vector<std::function<void(GLFWwindow* window, double xpos, double ypos)>> g_scrollCallBack;
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (g_fov >= 1.0f && g_fov <= 45.0f) {
		g_fov -= yoffset;
	}
	if (g_fov <= 1.0f) {
		g_fov = 1.0f;
	}
	if (g_fov >= 45.0f) {
		g_fov = 45.0f;
	}

	for (auto v : g_scrollCallBack) {
		v(window, xoffset, yoffset);
	}
}