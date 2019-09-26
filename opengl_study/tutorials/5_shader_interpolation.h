#pragma once
#include "../common.h"

static int tutorial5(){
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	// vertex data
	float vertices[] = {
		// position				// color
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, // left
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, // right
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, // top
	};

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 oColor;\n"
		"void main()\n{"
		"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"    oColor = aColor;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 oColor;\n"
		"void main()\n{"
		"    FragColor = vec4(oColor, 1.0f);\n"
		"}\0";

	// craete shader program
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	
	// VAO VBO
	unsigned int VAO, VBO;
	// gen
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind VAO
	glBindVertexArray(VAO);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set vertex attribute pointer
	// position pointer, offset is 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color pointer, offset 3 * sizeof(float)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// handle input event
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}