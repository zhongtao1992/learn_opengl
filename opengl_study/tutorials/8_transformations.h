#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"

// transform object
static int tutorial8() {
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	// vertex data
	float vertices[] = {
		// position				// colors				// texture coords
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f
	};
	// indice data
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aCoord;\n"
		"out vec3 oColor;\n"
		"out vec2 oCoord;\n"
		"uniform mat4 u_transform;"
		"void main()\n{"
		"    gl_Position = u_transform * vec4(aPos, 1.0f);\n"
		"    oColor = aColor;\n"
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

	unsigned int VAO, VBO, EBO;
	// gen
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind
	glBindVertexArray(VAO);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// set the position attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// set the color attributes pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// set the texture coord attributes pointers
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load image
	ImageLoader jpgLoader;
	jpgLoader.defaultLoadJPG("res/container.jpg");
	ImageLoader pngLoader;
	pngLoader.defaultLoadPNG("res/awesomeface.png");

	// set uniform
	program.use();
	program.setInt("u_texture", 0);
	program.setInt("u_texture2", 1);
	
	// rotation once
	/*glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0, 0, 1.0f));
	program.setMatrix4fv("u_transform", glm::value_ptr(trans));*/

	// combine order:1.scale��2.rotation��3.translate��code order:1.translate��2.rotation��3.scale
	// scale and rotation
	/*glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0, 0, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	program.setMatrix4fv("u_transform", glm::value_ptr(trans));*/

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// handle input event
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, jpgLoader.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pngLoader.getID());

		// draw triangle
		program.use();
		// transform
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0, 0, 1.0f));
		program.setMatrix4fv("u_transform", glm::value_ptr(trans));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// can't unbind EBO before unbind VAO
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
