#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"

static std::string des8() {
	return "Try to display only the center pixels of the texture image on the rectangle in such a way that the individual pixels are getting visible by changing the texture coordinates\n";
}

// load texture 
static int exercise8() {
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	printDes(des8().c_str());

	// vertex data
	float vertices[] = {
		// position				// colors				// texture coords
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.75f, 0.75f,   // top right
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		0.75f, 0.25f,   // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.25f, 0.25f,   // bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.25f, 0.75f     // top left
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
		"void main()\n{"
		"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"    oColor = aColor;\n"
		"    oCoord = aCoord;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 oCoord;\n"
		"uniform sampler2D u_texture;\n"
		"uniform sampler2D u_texture2;\n"
		"void main()\n{"
		"    FragColor = mix(texture(u_texture, oCoord), texture(u_texture2, vec2(1 - oCoord.x, oCoord.y)), 0.2);\n"
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
	jpgLoader.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	jpgLoader.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	jpgLoader.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	jpgLoader.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//jpgLoader.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//jpgLoader.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	jpgLoader.loadImg("res/container.jpg", GL_RGB);
	ImageLoader pngLoader;
	pngLoader.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	pngLoader.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	pngLoader.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pngLoader.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	pngLoader.loadImg("res/awesomeface.png", GL_RGBA);

	// set uniform
	program.use();
	program.setInt("u_texture", 0);
	program.setInt("u_texture2", 1);
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