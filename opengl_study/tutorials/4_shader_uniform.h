#pragma once
#include "../common.h"

// use uniform to change color per loop
static int tutorial4(){
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n{"
		"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n{"
		"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	const char* fragmentShaderSource2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 u_color;\n"
		"void main()\n{"
		"    FragColor = u_color;\n"
		"}\0";

	// create two shader program
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int shaderProgram2 = createShaderProgram(vertexShaderSource, fragmentShaderSource2);

	// vertex data
	float firstTriangleVer[] = {
		-1.0f, -0.5f, 0.0f, // left
		0.0f, -0.5f, 0.0f, // right
		-0.5f, 0.5f, 0.0f // top
	};
	float secondTriangleVer[] = {
		0.0f, -0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	// gen
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// bind first triangle info
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangleVer), firstTriangleVer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// bind second triangle info
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangleVer), secondTriangleVer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// handle input event
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw second triangle
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int uniformLocation = glGetUniformLocation(shaderProgram2, "u_color");
		if (uniformLocation != -1){
			glUseProgram(shaderProgram2);
			glUniform4f(uniformLocation, 0.0f, greenValue, 0.0f, 1.0f);
			glBindVertexArray(VAOs[1]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}