#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"
#include "../tools/Camera.h"

// 3D cube with depth test
static int tutorial15() {
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	Camera mCamera = Camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// register mouse event
	glfwSetCursorPosCallback(window, mouse_callback);
	// register scroll event
	glfwSetScrollCallback(window, scroll_callback);

	bool bFirstMouse = true;
	float fLastX = 0.0f;
	float fLastY = 0.0f;
	g_mouseCallBack.push_back([&](GLFWwindow* window, double xpos, double ypos) {
		if (bFirstMouse) {
			bFirstMouse = false;
			fLastX = xpos;
			fLastY = ypos;
		}
		float xoffset = xpos - fLastX;
		float yoffset = fLastY - ypos; // revert
		fLastX = xpos;
		fLastY = ypos;
		float sensitivity = 0.08f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		mCamera.mouseEvent(xoffset, yoffset);
	});
	g_scrollCallBack.push_back([&](GLFWwindow* window, double xoffset, double yoffset) {
		mCamera.scrollEvent(yoffset);
	});

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

	const char* objectVertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aCoord;\n"
		"out vec2 oCoord;\n"
		"uniform mat4 u_model;\n"
		"uniform mat4 u_view;\n"
		"uniform mat4 u_projection;\n"
		"void main()\n{"
		"    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);\n"
		"    oCoord = aCoord;\n"
		"}\0";
	const char* lampVertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aCoord;\n"
		"out vec2 oCoord;\n"
		"uniform mat4 u_model;\n"
		"uniform mat4 u_view;\n"
		"uniform mat4 u_projection;\n"
		"void main(){\n"
		"    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);\n"
		"    oCoord = aCoord;\n"
		"}\0";
	const char* objectFragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec3 u_objectColor;\n"
		"uniform vec3 u_lightColor;\n"
		"void main(){\n"
		"    FragColor = vec4(u_objectColor * u_lightColor, 1.0f);\n"
		"}\0";
	const char* lampFragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
		"    FragColor = vec4(1.0f);\n"
		"}\0";
	// create shader program
	GLShaderProgram program;
	program.initWithSource(objectVertexShaderSource, objectFragmentShaderSource);
	GLShaderProgram lampProgram;
	lampProgram.initWithSource(lampVertexShaderSource, lampFragmentShaderSource);

	unsigned int VAOs[2], VBOs[2];
	// gen
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// object cube
	// bind
	glBindVertexArray(VAOs[0]);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set the position attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// set the texture coord attributes pointers
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// light cube
	// bind
	glBindVertexArray(VAOs[1]);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
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
	program.setVec3("u_objectColor", 1.0f, 0.5f, 0.31f);
	program.setVec3("u_lightColor", 1.0f, 1.0f, 1.0f);

	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;
	// enable depth test
	glEnable(GL_DEPTH_TEST);
	// render loop
	while (!glfwWindowShouldClose(window)) {
		currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		float moveSpeed = 2.5f * deltaTime; // smooth the velocity
		mCamera.updateSpeed(moveSpeed);

		// handle input event
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			// forward
			mCamera.moveCamera(Camera::CameraDir::FORWARD);
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			// backward
			mCamera.moveCamera(Camera::CameraDir::BACKWARD);
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			// left
			mCamera.moveCamera(Camera::CameraDir::LEFT);
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			// right
			mCamera.moveCamera(Camera::CameraDir::RIGHT);
		}

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, jpgLoader.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pngLoader.getID());

		// draw object cube
		program.use();
		// MVP matrix
		// model matrix: basic transform
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		// view matrix: move container backward
		glm::mat4 view = mCamera.getViewMatrix();
		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getZoom()), g_viewWidth / g_viewHeight, 0.1f, 100.0f);
		program.setMatrix4fv("u_model", glm::value_ptr(model));
		program.setMatrix4fv("u_view", glm::value_ptr(view));
		program.setMatrix4fv("u_projection", glm::value_ptr(projection));
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// draw light cube
		lampProgram.use();
		// MVP matrix
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(1.2f, 1.0f, 2.0f));
		model2 = glm::scale(model2, glm::vec3(0.2f));
		lampProgram.setMatrix4fv("u_model", glm::value_ptr(model2));
		lampProgram.setMatrix4fv("u_view", glm::value_ptr(view));
		lampProgram.setMatrix4fv("u_projection", glm::value_ptr(projection));
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}