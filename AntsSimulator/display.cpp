#include "texture_load.h"
#include "ant.h"
#include "nest.h"
#include "trace.h"
#include "food.h"
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using glm::mat4;
using glm::translate;
using glm::vec3;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
void computeMatricesFromInputs(GLFWwindow* window);
mat4 getProjectionMatrix();
mat4 getViewMatrix();
vec3 Ants_Cordinats_To_Vec3(double x, double y);


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

	GLFWwindow* window = glfwCreateWindow(2560, 1440, "Ant Simulator", glfwGetPrimaryMonitor(), nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to open GLFW window, If you have an Intel GPU, they are not compatible with OpenGL 3.3.";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW\n";
		return -1;
	}
	const GLfloat G_Vertex_Buffer_Data_Ants[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f - 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f - 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f - 0.0f,

		-0.5f, 0.5f,0.0f,		0.0f, 1.0f - 1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f - 1.0f,
		0.5f, -0.5f,0.0f,		1.0f, 1.0f - 0.0f,
	};


	const GLfloat G_Vertex_Buffer_Data_Nest[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f - 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f - 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f - 0.0f,

		-0.5f, 0.5f,0.0f,		0.0f, 1.0f - 1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f - 1.0f,
		0.5f, -0.5f,0.0f,		1.0f, 1.0f - 0.0f,
	};

	
	GLuint Program_ID = LoadShaders("Resources//SimpleVertexShader.vertexshader",
                                   "Resources//SimpleFragmentShader.fragmentshader");
	
	GLuint Texture_Ants = CreateTextue("Resources//ant.jpg");
	GLuint Texture_Nest = CreateTextue("Resources//nest.jpg");
	GLuint TextureLocation = glGetUniformLocation(Program_ID, "myTextureSampler");
	
	GLuint VAO_Ants;
	glGenVertexArrays(1, &VAO_Ants);
	glBindVertexArray(VAO_Ants);

	GLuint Vertex_Buffer_Ants;
	glGenBuffers(1, &Vertex_Buffer_Ants);
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_Buffer_Ants);
	glBufferData(GL_ARRAY_BUFFER, sizeof(G_Vertex_Buffer_Data_Ants), G_Vertex_Buffer_Data_Ants, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	


	GLuint VAO_Nest;
	glGenVertexArrays(1, &VAO_Nest);
	glBindVertexArray(VAO_Nest);
	
	GLuint Vertex_Buffer_nest;
	glGenBuffers(1, &Vertex_Buffer_nest);
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_Buffer_nest);
	glBufferData(GL_ARRAY_BUFFER, sizeof(G_Vertex_Buffer_Data_Nest), G_Vertex_Buffer_Data_Nest, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetCursorPos(window, 2560 / 2, 1440 / 2);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	mat4 Ant_Model = mat4(1.0f);
	mat4 View = mat4(1.0f);
	mat4 Projection;

	View = glm::lookAt(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1, 0));
	Projection = glm::perspective(glm::radians(45.0f), 2560.0f / 1440.0f, 0.1f, 10000.0f);

	GLuint Model_Location = glGetUniformLocation(Program_ID, "model");
	GLuint View_Loaction = glGetUniformLocation(Program_ID, "view");
	GLuint Projection_Loaction = glGetUniformLocation(Program_ID, "projection");



	std::vector<Trace> Traces;
	Nest Nest;

	Ant Ants[] = {
		Ant(1500.0,700.0,45.0,Traces,Nest),
		Ant(970.0,700.0,135.0,Traces,Nest),
		Ant(2000.0,380.0,225.0,Traces,Nest),
		Ant(970.0,380.0,315.0,Traces,Nest),
		Ant(1100.0,380.0,90.0,Traces,Nest),
		Ant(1150.0,400.0,125.0,Traces,Nest),
		Ant(1200.0,420.0,160.0,Traces,Nest),
		Ant(1250.0,440.0,195.0,Traces,Nest),
		Ant(1350.0,460.0,220.0,Traces,Nest),
		Ant(1400.0,480.0,255.0,Traces,Nest),
		Ant(1450.0,500.0,290.0,Traces,Nest),
	};
	std::vector<Food>Foods; 
	Foods.push_back(Food(1800.0, 800.0, 15, 50.0));
	



	vec3 Nest_Position(0.0f, 0.0f, 0.0f);
	mat4 Nest_Model = mat4(1.0f);
	Nest_Model = glm::scale(Nest_Model, vec3(0.5, 0.5, 0.5));


	do
	{
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(Program_ID);


		glUniformMatrix4fv(Model_Location, 1, GL_FALSE, glm::value_ptr(Nest_Model));
		glUniformMatrix4fv(View_Loaction, 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(Projection_Loaction, 1, GL_FALSE, glm::value_ptr(Projection));

		glBindTexture(GL_TEXTURE_2D, Texture_Nest);
		glBindVertexArray(VAO_Nest);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, Texture_Ants);
		glBindVertexArray(VAO_Ants);
		for (auto& ant : Ants)
		{
			Ant_Model = mat4(1.0f);
			Ant_Model = translate(Ant_Model, Ants_Cordinats_To_Vec3(ant.GetX(), ant.GetY()));
			Ant_Model = glm::scale(Ant_Model, glm::vec3(0.2, 0.2, 0.2));
			glUniformMatrix4fv(Model_Location, 1, GL_FALSE, glm::value_ptr(Ant_Model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
			ant.Move(Traces,Foods);
		}
		
		glfwSwapBuffers(window);
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}


vec3 Ants_Cordinats_To_Vec3(double X_Ant, double Y_Ant)
{
	float X;
	float Y; 
	if (X_Ant < 1280.0)
		X = -1.0 + (X_Ant / 1280.0);
	else
		X = (X_Ant)/1280.0 - 1;

	if (Y_Ant < 720.0)
		Y = -1.0 + (Y_Ant / 720.0);
	else
		Y = Y_Ant/720.0 - 1;
	return vec3(X, Y, 0.0f);
}