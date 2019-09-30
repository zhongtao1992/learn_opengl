#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"

// 3D cube with depth test
static int tutorial10() {
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	// vertex data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aCoord;\n"
		"out vec2 oCoord;\n"
		"uniform mat4 u_model;\n"
		"uniform mat4 u_view;\n"
		"uniform mat4 u_projection;\n"
		"void main()\n{"
		"    gl_Position = u_projection * u_view * u_model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"    oCoord = aCoord;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 oCoord;\n"
		"uniform sampler2D u_texture;\n"
		"uniform sampler2D u_texture2;\n"
		"void main()\n{"
		"    FragColor = mix(texture(u_texture, oCoord), texture(u_texture2, oCoord), 0.2);\n"
		"}\0";

	// create shader program
	GLShaderProgram program;
	program.initWithSource(vertexShaderSource, fragmentShaderSource);

	unsigned int VAO, VBO;
	// gen
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind
	glBindVertexArray(VAO);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set the position attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// set the texture coord attributes pointers
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load image
	ImageLoader jpgLoader;
	jpgLoader.defaultLoadJPG("res/container.jpg");
	ImageLoader pngLoader;
	pngLoader.defaultLoadPNG("res/awesomeface.png");

	// set uniform
	program.use();
	program.setInt("u_texture", 0);
	program.setInt("u_texture2", 1);

	// enable depth test
	glEnable(GL_DEPTH_TEST);
	// render loop
	while (!glfwWindowShouldClose(window)) {
		// handle input event
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, jpgLoader.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pngLoader.getID());

		// draw triangle
		program.use();

		// MVP matrix
		// model matrix: basic transform
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(50.0f) * (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		// view matrix: move container backward
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
		program.setMatrix4fv("u_model", glm::value_ptr(model));
		program.setMatrix4fv("u_view", glm::value_ptr(view));
		program.setMatrix4fv("u_projection", glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}