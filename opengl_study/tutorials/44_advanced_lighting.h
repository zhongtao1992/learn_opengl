#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"
#include "../tools/Camera.h"
#include "../render/Model.h"
#include <string>

// advanced lighting
static int tutorial44() {
	// config opengl
	GLFWwindow* window = nullptr;
	if (commonInit(&window) < 0) {
		return -1;
	}

	Camera mCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

	// create shader program
	GLShaderProgram program;
	program.initWithDiskFile("shader/44_advanced_lighting.vert", "shader/44_advanced_lighting.frag");

	// light pos
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	// vertex data
	float vertices[] = {
		// positions            // normals         // texcoords
		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	unsigned int VAO, VBO;
	// gen
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// vertex attribut position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// vertex attribut normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// vertex attribut texture coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// load image
	ImageLoader imgLoader;
	imgLoader.defaultLoadImg("res/wood.png");

	program.use();
	program.setInt("u_texture", 0);

	srand(glfwGetTime());

	// loop
	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;

	int idx = 0;
	// render loop
	while (!glfwWindowShouldClose(window)) {		
		currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		float moveSpeed = 3.5f * deltaTime; // smooth the velocity
		mCamera.updateSpeed(moveSpeed);

		// handle input event
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			// forward
			mCamera.moveCamera(Camera::CameraDir::FORWARD);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			// backward
			mCamera.moveCamera(Camera::CameraDir::BACKWARD);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			// left
			mCamera.moveCamera(Camera::CameraDir::LEFT);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			// right
			mCamera.moveCamera(Camera::CameraDir::RIGHT);
		}

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, imgLoader.getID());

		program.use();
		// MVP matrix
		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getZoom()), g_viewWidth / g_viewHeight, 0.1f, 100.0f);
		glm::mat4 view = mCamera.getLookAt();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(2.5, 2.5, 2.5));

		program.setMatrix4fv("u_view", glm::value_ptr(view));
		program.setMatrix4fv("u_projection", glm::value_ptr(projection));
		program.setMatrix4fv("u_model", glm::value_ptr(model));

		// light param
		program.setVec3("u_lightPos", lightPos);
		program.setVec3("u_viewPos", mCamera.getPosition());
		if (idx % 1100 == 0){
			program.setInt("u_bBlinn", 1);
			std::cout << "use blinn" << std::endl;
		}
		else if(idx % 2100 == 0){
			program.setInt("u_bBlinn", 0);
			std::cout << "use normal" << std::endl;
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();

		idx++;
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}