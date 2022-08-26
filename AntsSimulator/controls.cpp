#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>



glm::vec3 Position = glm::vec3(0, 0, 5);
float Horizontal_Angel = 3.14;
float Vertical_Angel = 0.0;
float Initial_FoV = 45.0;
float Speed = 3.0;
double Mouse_Speed = 0.05;
float Delta_Time;
float Last_Time = glfwGetTime();
float FoV = Initial_FoV;
glm::mat4 Projection_Matrix;
glm::mat4 View_Matrix;
void scroll_callback(GLFWwindow* Window, double X_Offset, double Y_Offset)
{
	FoV = Initial_FoV - 5 * Y_Offset;
}

void computeMatricesFromInputs(GLFWwindow* Window)
{
	double  X_Pos, Y_Pos;
	glfwGetCursorPos(Window, &X_Pos, &Y_Pos);
	glfwSetCursorPos(Window, 1920 / 2, 1080 / 2);

	Delta_Time = (glfwGetTime() - Last_Time);
	Last_Time = glfwGetTime();
	Horizontal_Angel += Mouse_Speed * Delta_Time * float(1920.0 / 2.0 - X_Pos);
	Vertical_Angel += Mouse_Speed * Delta_Time * float(1080.0 / 2.0 - Y_Pos);
	glm::vec3 Direction(
		cos(Vertical_Angel) * sin(Horizontal_Angel),
		sin(Vertical_Angel),
		cos(Vertical_Angel) * cos(Horizontal_Angel)
	);
	glm::vec3 Right = glm::vec3(
		sin(Horizontal_Angel - 3.14 / 2.0),
		0,
		cos(Horizontal_Angel - 3.14 / 2.0)
	);
	glm::vec3 Up = glm::cross(Right, Direction);

	// Move forward
	if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS) {
		Position += Direction * Delta_Time * Speed;
	}
	// Move backward
	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		Position -= Direction * Delta_Time * Speed;
	}
	// Strafe right
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		Position += Right * Delta_Time * Speed;
	}
	// Strafe left
	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		Position -= Right * Delta_Time * Speed;
	}
	glfwSetScrollCallback(Window, scroll_callback);
	Projection_Matrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	View_Matrix = glm::lookAt(
		Position,           // Camera is here
		Position + Direction, // and looks here : at the same position, plus "direction"
		Up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 getProjectionMatrix()
{
	return Projection_Matrix;
}

glm::mat4 getViewMatrix()
{
	return View_Matrix;
}