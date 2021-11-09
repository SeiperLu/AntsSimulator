#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>



glm::vec3 position = glm::vec3(0, 0, 5);
float horizontalAngel = 3.14;
float verticalAngel = 0.0;
float initialFoV = 45.0;
float speed = 3.0;
double mouseSpeed = 0.05;
float deltaTime;
float lastTime = glfwGetTime();
float FoV = initialFoV;
glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FoV = initialFoV - 5 * yoffset;
}

void computeMatricesFromInputs(GLFWwindow* window)
{
	double  xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, 1920 / 2, 1080 / 2);

	deltaTime = (glfwGetTime() - lastTime);
	lastTime = glfwGetTime();
	horizontalAngel += mouseSpeed * deltaTime * float(1920.0 / 2.0 - xpos);
	verticalAngel += mouseSpeed * deltaTime * float(1080.0 / 2.0 - ypos);
	glm::vec3 direction(
		cos(verticalAngel) * sin(horizontalAngel),
		sin(verticalAngel),
		cos(verticalAngel) * cos(horizontalAngel)
	);
	glm::vec3 right = glm::vec3(
		sin(horizontalAngel - 3.14 / 2.0),
		0,
		cos(horizontalAngel - 3.14 / 2.0)
	);
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	glfwSetScrollCallback(window, scroll_callback);
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

glm::mat4 getViewMatrix()
{
	return ViewMatrix;
}