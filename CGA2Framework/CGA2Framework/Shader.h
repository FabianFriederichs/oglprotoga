#pragma once
#include "headers.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "IDProvider.h"
#include "Texture.h"

enum UniformType {UniTf, UniTi, UniTui, UniTfv, UniTiv, UniTuiv, UniTmfv};

class Shader
{
public:
	Shader()
	{

	}

	virtual ~Shader()
	{
		glDeleteProgram(this->Program);
	}

	//virtual functions to set pass-specific states while Rendering
	virtual void preRenderActions() {};
	virtual void postRenderActions() {};

	virtual void setOutput(bool totexture, std::vector<Texture*>* = nullptr) {};

	// The program ID
	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) : m_id(IDProvider::createID())
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();
		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDetachShader(this->Program, vertex);
		glDetachShader(this->Program, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}

	bool isActive()
	{
		GLint progName = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &progName);
		if (progName != this->Program)
			return false;
		return true;
	}

private:
	GLint getUniformLocation(std::string name)
	{
		return glGetUniformLocation(this->Program, name.c_str());
	}
	GLint m_id;
public:
	template<class T> 
	inline void setUniform(const std::string name, T stuff);
	template<class T>
	inline void setUniform(const std::string name, T stuff, const GLboolean transpose);

	GLint getID() { return m_id; }
};


//void setUniform(std::string name, UniformType t, iterator )
//template<class T> 
//void Shader::setUniform(const std::string name, T& stuff)
//{
//	std::cout << "NON SUPPORTED TYPE";
//
//}

template<>
inline void Shader::setUniform(const std::string name, const GLfloat value)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
		glUseProgram(this->Program);
	glUniform1f(loc, value);

}

template<> 
inline void Shader::setUniform(const std::string name, std::vector<GLfloat> &values)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
		glUseProgram(this->Program);
	auto size = values.size();
	switch (size)
	{
		case 2:
			glUniform1fv(loc, 2, values.data());
			break;
		case 3:
			glUniform1fv(loc, 3, values.data());
			break;
		case 4:
			glUniform1fv(loc, 4, values.data());
			break;
		default:
			std::cout << "VECTOR TOO LARGE OR < 2 VALUES";
	}
}

template<>
inline void Shader::setUniform(const std::string name, const GLint value)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
		glUseProgram(this->Program);
	glUniform1i(loc, value);
}

template<>
inline void Shader::setUniform(const std::string name, glm::mat4 &value, const GLboolean transpose)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
		glUseProgram(this->Program);
	glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(value));
}