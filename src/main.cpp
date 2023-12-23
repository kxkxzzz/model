#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "common.h"
#include "skybox.h"
#include "shaders.h"
#include "camera.h"
#include "Model.h"

const std::vector<std::string> skyboxTextSrc {
	"texture/skybox/posx.jpg", "texture/skybox/negx.jpg",
	"texture/skybox/posy.jpg",   "texture/skybox/negy.jpg",
	"texture/skybox/posz.jpg", "texture/skybox/negz.jpg"
};


void DrawModel(Shader& modelShader, Model& ourModel, glm::mat4& model, glm::mat4& projection, glm::mat4& view);

int main() {

	GLFWwindow* window;

	if (init(window) == -1) {
		glCheckError();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	Skybox skybox(skyboxTextSrc);
	Shader modelShader("shader/ModelLoading.vs", "shader/ModelLoading.fs");
	Model ourModel("model/nanosuit/nanosuit.obj");

	glCheckError();

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

		DrawModel(modelShader, ourModel, model, projection, view);

		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skybox.Draw(view, projection);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glCheckError();

	glfwTerminate();

	return 0;
}


void DrawModel(Shader& modelShader, Model& ourModel, glm::mat4& model, glm::mat4& projection, glm::mat4& view) {
	modelShader.use();
	modelShader.setMat4("model", model);
	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);
	ourModel.Draw(modelShader);
}