#pragma once
#include "../common.h"
#include "../tools/GLShaderProgram.h"
static std::string des5() {
	return "Output the vertex position to the fragment shader using the out keyword and set the fragment's color equal to this vertex position\n";
}

static int exercise5(){
	// config glfw
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}
	printDes(des5().c_str());

	// shader source
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 o_Color;\n"
		"void main()\n{"
		"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"    o_Color = gl_Position;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 o_Color;\n"
		"void main()\n{"
		"    FragColor = o_Color;\n"
		"}\0";
	// create shader program
	GLShaderProgram program;
	program.initWithSource(vertexShaderSource, fragmentShaderSource);

	// vertex data
	float firstTriangleVer[] = {
		-0.5f, -0.5f, 0.0f, // left
		0.5f, -0.5f, 0.0f, // right
		0.0f, 0.5f, 0.0f // top
	};

	unsigned int VAO, VBO;
	// gen 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangleVer), firstTriangleVer, GL_STATIC_DRAW);
	// vertex attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		program.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap buffer and handle event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}