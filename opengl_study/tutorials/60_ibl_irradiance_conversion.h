#pragma once
#include "../common.h"
#include "../tools/ImageLoader.h"
#include "../tools/GLShaderProgram.h"
#include "../tools/Camera.h"
#include "../render/Model.h"
#include <string>
#include "../mesh/SphereMesh.h"

// ibl
static int tutorial60() {
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
	program.initWithDiskFile("shader/58_lighting.vert", "shader/58_lighting.frag");
	GLShaderProgram equirectangularToCubemapShader;
	equirectangularToCubemapShader.initWithDiskFile("shader/60_cubemap.vert", "shader/60_cubemap.frag");
	GLShaderProgram backgroundShader;
	backgroundShader.initWithDiskFile("shader/60_background.vert", "shader/60_background.frag");

	program.use();
	program.setVec3("u_albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	program.setFloat("u_ao", 1.0f);
	backgroundShader.use();
	backgroundShader.setInt("u_environmentMap", 0);

	float cubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	CubeMesh cube(cubeVertices, sizeof(cubeVertices));
	// position attribute
	cube.setAttribute(0, 3, 8 * sizeof(float), (void*)0);
	// normal attribute
	cube.setAttribute(1, 3, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// texture coordinate attribute
	cube.setAttribute(2, 2, 8 * sizeof(float), (void*)(6 * sizeof(float)));

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

	// pbr framebuffer
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	// load image
	ImageLoader imgLoader;
	imgLoader.loadHDR("res/hdr/newport_loft.hdr");

	// pbr cubemap
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr projection and view matrix
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// pbr convert hdr equirectangular environment map to cubemap
	equirectangularToCubemapShader.use();
	equirectangularToCubemapShader.setInt("u_equirectangularMap", 0);
	equirectangularToCubemapShader.setMatrix4fv("u_projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, imgLoader.getID());

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i){
		equirectangularToCubemapShader.setMatrix4fv("u_view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.draw(equirectangularToCubemapShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// static shader uniform
	glm::mat4 projection = glm::perspective(glm::radians(mCamera.getZoom()), g_viewWidth / g_viewHeight, 0.1f, 100.0f);
	program.use();
	program.setMatrix4fv("u_projection", projection);
	backgroundShader.use();
	backgroundShader.setMatrix4fv("u_projection", projection);

	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

	// loop
	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
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

		//glm::mat4 projection = glm::perspective(glm::radians(mCamera.getZoom()), (float)g_viewWidth / (float)g_viewHeight, 0.1f, 100.0f);
		glm::mat4 view = mCamera.getViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();
		program.setMatrix4fv("u_view", view);
		program.setVec3("u_camPos", mCamera.getPosition());

		// render sphere
		for (int row = 0; row < nrRows; ++row) {
			program.setFloat("u_metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col) {
				program.setFloat("u_roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
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

		// render skybox
		backgroundShader.use();
		backgroundShader.setMatrix4fv("u_view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		cube.draw(backgroundShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}