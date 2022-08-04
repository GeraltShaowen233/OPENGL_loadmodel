#include<iostream>

#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include"Material.h"
#include"Camera.h"
#include"LightDirectional.h"
#include"LightPoint.h"
#include"LightSpot.h"
#include "Mesh.h"
#include"Model.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera camera(glm::vec3(0, 10.0f, 3.0f), 10.0f, -80.0f, glm::vec3(0, 1.0f, 0));
LightDirectional light(glm::vec3(1.0f,1.0f,-1.0f),glm::vec3(0, glm::radians(120.0f),0));
LightPoint lightP0 (glm::vec3(-5.0f, 10.0f, 5.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0));
float deltaTime = 0.0f;	
float lastFrame = 0.0f;



#pragma region Input Declare
float lastX;
float lastY;
bool firstMouse = true;



void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.speedZ = 1.0f* deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.speedZ = -1.0f * deltaTime;
	}
	else
	{
		camera.speedZ = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.speedX = 1.0f * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.speedX = -1.0f * deltaTime;
	}
	else
	{
		camera.speedX = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.speedY = -1.0f * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.speedY = 1.0f * deltaTime;
	}
	else
	{
		camera.speedY = 0;
	}

}

float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse == true)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xpos - lastX;
	deltaY = ypos - lastY;

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(deltaX, deltaY);
}

#pragma endregion


int main(int argc,char* argv[])
{
	std::string exepath = argv[0];
	printf("%f %f %f", light.direction.x, light.direction.y, light.direction.z);
#pragma region open a window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindow 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 可编程流水线

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL)
	{
		printf("open window failed");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//use it as current window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("INIT GLEW FAILED");
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

#pragma endregion

	Shader* myShader = new Shader("VertexSource.vert", "fragmentSource.frag");
	Shader* cubeShader = new Shader("cube.vert", "cube.frag");


#pragma region Init and Load Models to VAO,VBO
	bool fbx = true;
	//Model Model(exepath.substr(0,exepath.find_last_of('\\'))+"\\model\\nanosuit\\nanosuit.obj");
	//Model Model(exepath.substr(0, exepath.find_last_of('\\')) + "\\model\\shenhe\\Dorka.fbx");
	Model Model(exepath.substr(0, exepath.find_last_of('\\')) + "\\model\\mona2\\mona.fbx");

	unsigned int VBO,lightcubeVAO;
	glGenVertexArrays(1, &lightcubeVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(lightcubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

#pragma endregion


#pragma region MVP
	glm::mat4 model;
	model = glm::mat4(1.0f);
	glm::mat4 view;
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
#pragma endregion
	double previousTime = glfwGetTime();
	int frameCount = 0;
	while (!glfwWindowShouldClose(window))
	{
		model = glm::mat4(1.0f);
		if (fbx == true)
		{
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0, 0));
			model = glm::scale(model, glm::vec3(10.0f));
		}
		
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		double currentTime = glfwGetTime();
		frameCount++;
		// If a second has passed.
		if (currentTime - previousTime >= 1.0)
		{
			// Display the frame count here any way you want.
			printf("%d\n", frameCount);

			frameCount = 0;
			previousTime = currentTime;
		}

		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		view = camera.GetViewMatrix();

		myShader->use();
			
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glUniform3f(glGetUniformLocation(myShader->ID, "lightD.color"), light.color.x, light.color.y, light.color.z);
		glUniform3f(glGetUniformLocation(myShader->ID, "lightD.pos"),light.position.x, light.position.y, light.position.z);
		glUniform3f(glGetUniformLocation(myShader->ID, "lightD.dirToLight"), light.direction.x, light.direction.y, light.direction.z);

		glUniform3f(glGetUniformLocation(myShader->ID, "lightP0.color"), lightP0.color.x, lightP0.color.y, lightP0.color.z);
		glUniform3f(glGetUniformLocation(myShader->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);
		glUniform3f(glGetUniformLocation(myShader->ID, "lightP0.dirToLight"), lightP0.direction.x, lightP0.direction.y, lightP0.direction.z);
		glUniform1f(glGetUniformLocation(myShader->ID, "lightP0.constant"), lightP0.constant);
		glUniform1f(glGetUniformLocation(myShader->ID, "lightP0.linear"), lightP0.linear);
		glUniform1f(glGetUniformLocation(myShader->ID, "lightP0.quadratic"), lightP0.quadratic);

			//glBindVertexArray(VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			//cube.Draw(myMaterial->shader);
		Model.draw(myShader);

		cubeShader->use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightP0.position);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(lightcubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		glfwSwapBuffers(window);// double buffer
		glfwPollEvents();// get the input from user
		camera.UpdateCameraPos();
		
	}
	glfwTerminate();

	return 0;


}

