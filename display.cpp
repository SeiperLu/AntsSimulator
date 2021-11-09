#include "texture_load.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using glm::mat4;
using glm::translate;
using glm::vec3;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
void computeMatricesFromInputs(GLFWwindow* window);
mat4 getProjectionMatrix();
mat4 getViewMatrix();


int main() {
	glewExperimental = true;
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW\n";
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(1920, 1080, "Ant Simulator", nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW window, If you have an Intel GPU, they are not compatible with OpenGL 3.3.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	const GLfloat g_vertex_buffer_data_ants[] = {
	-1.0f,-1.0f,0.0f, 
	-1.0f,0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 

	-1.0f, 0.0f,0.0f, 
	0.0f,0.0f,0.0f,
	0.0f, -1.0f,0.0f, 
	};
	
	const GLfloat g_uv_buffer_data_ants[] = {
	0.0f, 1.0f - 0.0f,
	0.0f, 1.0f - 1.0f,
	1.0f, 1.0f - 0.0f,

	0.0f, 1.0f - 1.0f,
	1.0f, 1.0f - 1.0f,
	1.0f, 1.0f - 0.0f,
	};

	GLuint vertexbuffer_ants;
	glGenBuffers(1, &vertexbuffer_ants);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_ants);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_ants), g_vertex_buffer_data_ants, GL_STATIC_DRAW);

	GLuint uv_buffer_ants;
	glGenBuffers(1, &uv_buffer_ants);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_ants);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_ants), g_uv_buffer_data_ants, GL_STATIC_DRAW);

	GLuint programID = LoadShaders("D://Projekty//Ant_Simulator//Ants//SimpleVertexShader.vertexshader", "D://Projekty//Ant_Simulator//Ants//SimpleFragmentShader.fragmentshader");
	
	GLuint Texture = CreateTextue("D://Projekty//Ant_Simulator//Ants//ant.jpg");
	GLuint TextureLocation = glGetUniformLocation(programID, "myTextureSampler");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetCursorPos(window, 1920 / 2, 1080 / 2);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	do
	{
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureLocation, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_ants);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_ants);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}
