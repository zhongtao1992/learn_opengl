#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"
#include "../tools/Camera.h"
#include <string>

// multiple light
static int tutorial25() {
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

	// light pos
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	// vertex data
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// create shader program
	GLShaderProgram program;
	program.initWithDiskFile("shader/25_material_object.vert", "shader/25_material_object.frag");
	GLShaderProgram lampProgram;
	lampProgram.initWithDiskFile("shader/18_material_lamp.vert", "shader/18_material_lamp.frag");

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// set the normal vector attributes pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// set the diffuse maps coord attributes pointers
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// light cube
	// bind
	glBindVertexArray(VAOs[1]);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set the position attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// set the normal vector coord attributes pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load diffuse maps
	ImageLoader diffuseMap;
	diffuseMap.defaultLoadPNG("res/container2.png");
	// load specular maps
	ImageLoader specularMap;
	specularMap.defaultLoadPNG("res/container2_specular.png");

	// set uniform
	program.use();
	// set material
	program.setInt("u_material.diffuse", 0);
	program.setInt("u_material.specular", 1);
	program.setFloat("u_material.shininess", 32.0f);
	// direction light param
	program.setVec3("u_dirLight.direction", -0.2f, -1.0f, -0.3f);
	program.setVec3("u_dirLight.ambient", 0.05f, 0.05f, 0.05f);
	program.setVec3("u_dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	program.setVec3("u_dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light param
	for (int i = 0; i < 4; ++i){
		std::string idx = std::to_string(i);
		
		std::string key = "u_pointLights[" + idx + "].position";
		program.setVec3(key.c_str(), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);

		key = "u_pointLights[" + idx + "].constant";
		program.setFloat(key.c_str(), 1.0f);
		key = "u_pointLights[" + idx + "].linear";
		program.setFloat(key.c_str(), 0.09f);
		key = "u_pointLights[" + idx + "].quadratic";
		program.setFloat(key.c_str(), 0.032f);

		key = "u_pointLights[" + idx + "].ambient";
		program.setVec3(key.c_str(), 0.05f, 0.05f, 0.05f);
		key = "u_pointLights[" + idx + "].diffuse";
		program.setVec3(key.c_str(), 0.8f, 0.8f, 0.8f);
		key = "u_pointLights[" + idx + "].specular";
		program.setVec3(key.c_str(), 1.0f, 1.0f, 1.0f);
	}	

	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// enable depth test
	glEnable(GL_DEPTH_TEST);
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
		glBindTexture(GL_TEXTURE_2D, diffuseMap.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap.getID());

		// draw object cube
		program.use();
		// MVP matrix
		// model matrix: basic transform
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		// view matrix: move container backward
		glm::mat4 view = mCamera.getViewMatrix();
		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getZoom()), g_viewWidth / g_viewHeight, 0.1f, 100.0f);
		program.setMatrix4fv("u_view", glm::value_ptr(view));
		program.setMatrix4fv("u_projection", glm::value_ptr(projection));
		glm::vec3 viewPos = mCamera.getPosition();
		program.setVec3("u_viewPos", viewPos.x, viewPos.y, viewPos.z);

		// spot light param
		glm::vec3 cameraPos = mCamera.getPosition();
		glm::vec3 front = mCamera.getFront();
		program.setVec3("u_spotLight.position", cameraPos.x, cameraPos.y, cameraPos.z);
		program.setVec3("u_spotLight.direction", front.x, front.y, front.z);
		program.setVec3("u_spotLight.ambient", 0.0f, 0.0f, 0.0f);
		program.setVec3("u_spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		program.setVec3("u_spotLight.specular", 1.0f, 1.0f, 1.0f);
		program.setFloat("u_spotLight.constant", 1.0f);
		program.setFloat("u_spotLight.linear", 1.0f);
		program.setFloat("u_spotLight.quadratic", 1.0f);
		program.setFloat("u_spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		program.setFloat("u_spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// draw more cube
		for (int i = 0; i < 10; ++i) {
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			program.setMatrix4fv("u_model", glm::value_ptr(model));
			glBindVertexArray(VAOs[0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// draw light cube
		lampProgram.use();		
		lampProgram.setMatrix4fv("u_view", glm::value_ptr(view));
		lampProgram.setMatrix4fv("u_projection", glm::value_ptr(projection));
		for (int i = 0; i < 4; ++i){
			// MVP matrix
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, pointLightPositions[i]);
			model2 = glm::scale(model2, glm::vec3(0.2f));
			lampProgram.setMatrix4fv("u_model", glm::value_ptr(model2));

			glBindVertexArray(VAOs[1]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}