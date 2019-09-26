#pragma once
#include "../common.h"
#include "../tools/GLShaderProgram.h"
static std::string des4() {
	return "Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex shader using this offset value\n";
}

static int exercise4(){
	// config glfw
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}
	printDes(des4().c_str());

	// shader source
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform float u_fOffset;\n"
		"void main()\n{"
		"    gl_Position = vec4(aPos.x + u_fOffset, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n{"
		"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

	float fMoveValue = 0.0f;
	// render loop
	while (!glfwWindowShouldClose(window)){
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		program.use();
		program.setFloat("u_fOffset", fMoveValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap buffer and handle event
		glfwSwapBuffers(window);
		glfwPollEvents();

		fMoveValue += 0.00001f;
		if (fMoveValue > 0.5){
			fMoveValue = 0.0f;
		}
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}