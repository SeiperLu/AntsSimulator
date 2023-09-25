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

#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using glm::mat4;
using glm::translate;
using glm::vec3;
std::vector<Food> Foods;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


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
	glViewport(0, 0, 2560, 1440);
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

	const GLfloat G_Vertex_Buffer_Data_Food[] = {
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
	GLuint Texture_Food = CreateTextue("Resources//food.jpg");
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
	
	GLuint Vertex_Buffer_Nest;
	glGenBuffers(1, &Vertex_Buffer_Nest);
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_Buffer_Nest);
	glBufferData(GL_ARRAY_BUFFER, sizeof(G_Vertex_Buffer_Data_Nest), G_Vertex_Buffer_Data_Nest, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	GLuint VAO_Food;
	glGenVertexArrays(1, &VAO_Food);
	glBindVertexArray(VAO_Food);

	GLuint Vertex_Buffer_Food;
	glGenBuffers(1, &Vertex_Buffer_Food);
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_Buffer_Food);
	glBufferData(GL_ARRAY_BUFFER, sizeof(G_Vertex_Buffer_Data_Food), G_Vertex_Buffer_Data_Food, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetCursorPos(window, 2560 / 2, 1440 / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	GLuint Model_Location = glGetUniformLocation(Program_ID, "model");
	GLuint View_Loaction = glGetUniformLocation(Program_ID, "view");
	GLuint Projection_Loaction = glGetUniformLocation(Program_ID, "projection");



	std::vector<Trace> Traces;
	Nest Nest(1280.0,720.0, 0);

	Ant Ants[] = {
		Ant(2560.0,1440.0,45.0,Traces,Nest),
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
	
	
	
	Foods.push_back(Food(2200.0, 1100.0, 70, 100.0));
	vec3 Nest_Position(1280.0f, 720.0f, 0.2f);
	
	
	mat4 Nest_Model = mat4(1.0f);
	Nest_Model = translate(Nest_Model, Nest_Position);
	Nest_Model = glm::scale(Nest_Model, vec3(75.0f, 75.0f, 75.0f));
	mat4 Ant_Model = mat4(1.0f);
	mat4 Food_Model = mat4(1.0f);
	mat4 View = mat4(1.0f);
	mat4 Projection = glm::ortho(0.0f, 2560.0f, 0.0f, 1440.0f, 0.0f, 100.0f);
	View = translate(View, vec3(0.0f, 0.0f, -3.0f));
	
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

		glBindTexture(GL_TEXTURE_2D, Texture_Food);
		glBindVertexArray(VAO_Food);
		for (auto& food : Foods )
		{
			Food_Model = mat4(1.0f);
			Food_Model = translate(Food_Model, vec3((float)food.GetX(), (float)food.GetY(), 0.2f));
			Food_Model = glm::scale(Food_Model, glm::vec3(50.0f, 50.0f, 50.0f));
			glUniformMatrix4fv(Model_Location, 1, GL_FALSE, glm::value_ptr(Food_Model));
			glUniformMatrix4fv(View_Loaction, 1, GL_FALSE, glm::value_ptr(View));
			glUniformMatrix4fv(Projection_Loaction, 1, GL_FALSE, glm::value_ptr(Projection));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindTexture(GL_TEXTURE_2D, Texture_Ants);
		glBindVertexArray(VAO_Ants);
		for (auto& ant : Ants)
		{
			Ant_Model = mat4(1.0f);
			Ant_Model = translate(Ant_Model, vec3((float)ant.GetX(), (float)ant.GetY(), 0.2f));
			Ant_Model = glm::rotate(Ant_Model,glm::radians(ant.GetTheta()) - (float)M_PI_2, glm::vec3(0.0f, 0.0f, 1.0f));
			Ant_Model = glm::scale(Ant_Model, glm::vec3(50.0f, 50.0f, 50.0f));
			glUniformMatrix4fv(Model_Location, 1, GL_FALSE, glm::value_ptr(Ant_Model));
			glUniformMatrix4fv(View_Loaction, 1, GL_FALSE, glm::value_ptr(View));
			glUniformMatrix4fv(Projection_Loaction, 1, GL_FALSE, glm::value_ptr(Projection));
			glDrawArrays(GL_TRIANGLES, 0, 6);
			ant.Move(Traces, Foods);
		}
		glfwSwapBuffers(window);
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xPos, yPos;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xPos, &yPos);
		Foods.push_back(Food(xPos, 1440.0 - yPos, 70.0, 100.0));
	}
}