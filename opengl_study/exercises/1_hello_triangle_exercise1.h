#pragma once
#include "../common.h"
static std::string des1(){
	return "draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data\n";
}

// draw a triangle
static int exercise1(){
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	printDes(des1().c_str());

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

	// create shader program
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	// vertex data
	float vertices[] = {
		// first triangle
		-1.0f, -0.5f, 0.0f, // left
		0.0f, -0.5f, 0.0f, // right
		-0.5f, 0.5f, 0.0f, // top
		// second triangle
		0.0f, -0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the vertex array object
	glBindVertexArray(VAO);
	// bind the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// fill data uage:GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// tell opengl how it should interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind VAO
	glBindVertexArray(0);

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
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}