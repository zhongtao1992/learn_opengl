#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"
#include "../tools/Camera.h"
#include "../render/Model.h"
#include <string>
#include "../mesh/SphereMesh.h"

// lighting
static int tutorial59() {
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
	program.initWithDiskFile("shader/58_lighting.vert", "shader/59_lighting_textured.frag");
	program.use();
	program.setVec3("u_albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	program.setFloat("u_ao", 1.0f);
	program.setInt("u_albedoMap", 0);
	program.setInt("u_normalMap", 1);
	program.setInt("u_matallicMap", 2);
	program.setInt("u_roughnessMap", 3);
	program.setInt("u_aoMap", 4);

	ImageLoader albedoTex;
	albedoTex.loadImg("res/pbr/rusted_iron/albedo.png");
	ImageLoader normalTex;
	normalTex.loadImg("res/pbr/rusted_iron/normal.png");
	ImageLoader metallicTex;
	metallicTex.loadImg("res/pbr/rusted_iron/metallic.png");
	ImageLoader roughnessTex;
	roughnessTex.loadImg("res/pbr/rusted_iron/roughness.png");
	ImageLoader aoTex;
	aoTex.loadImg("res/pbr/rusted_iron/ao.png");

	// light info
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	SphereMesh sphere = SphereMesh(64, 64);

	// load image
	ImageLoader imgLoader;
	imgLoader.defaultLoadImg("res/wood.png");

	program.use();
	program.setInt("u_texture", 0);

	srand(glfwGetTime());

	// loop
	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedoTex.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalTex.getID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallicTex.getID());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughnessTex.getID());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, aoTex.getID());

		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getZoom()), (float)g_viewWidth / (float)g_viewHeight, 0.1f, 100.0f);
		glm::mat4 view = mCamera.getViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();
		program.setMatrix4fv("u_projection", projection);
		program.setMatrix4fv("u_view", view);
		program.setVec3("u_camPos", mCamera.getPosition());

		// render sphere
		for (int row = 0; row < nrRows; ++row) {
			for (int col = 0; col < nrColumns; ++col) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				program.setMatrix4fv("u_model", model);
				sphere.draw(program);
			}
		}

		// light source
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			program.setVec3(("u_lightPositions[" + std::to_string(i) + "]").c_str(), newPos);
			program.setVec3(("u_lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			program.setMatrix4fv("u_model", model);
			sphere.draw(program);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}