#include "Shader.h"
#include<iostream>
#include<fstream>
#include<sstream>

#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::stringstream vertexSStream;
	std::stringstream fragmentSStream;
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	vertexFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	try
	{
		if (!vertexFile.is_open()||!fragmentFile.is_open())
		{
			throw std::exception("open file error");
		}
		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();

		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		checkCompileErrors(ID, "PROGRAM");

	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}


}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::SetUniform3f(const char* paraNameString, glm::vec3 param)
{
	glUniform3f(glGetUniformLocation(ID, paraNameString), param.x, param.y, param.z);
}

void Shader::SetUniform1f(const char* paraNameString, float param)
{
	glUniform1f(glGetUniformLocation(ID, paraNameString), param);
}

void Shader::SetUniform1i(const char* paraNameString, int slot)
{
	glUniform1i(glGetUniformLocation(ID, paraNameString), slot);
}

void Shader::checkCompileErrors(unsigned int ID, std::string type)
{
	int success;
	char infoLog[512];

	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infoLog);
			std::cout << "shader compile error: " << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "program link error: " << infoLog << std::endl;
		}

	}
}
