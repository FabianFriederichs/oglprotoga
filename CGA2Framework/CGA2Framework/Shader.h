#pragma once
#include "headers.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "IDProvider.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "glerror.h"

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

	virtual bool setOutput(FrameBuffer* _buf = nullptr)
	{
		if (_buf == nullptr)
		{
			_target = nullptr;
			glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				return false;
			else
				return true;
		}
		else
		{
			_target = _buf;
			return _buf->bind();
		}
	}

	// The program ID
	GLuint Program;
	FrameBuffer* _target;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) : m_id(IDProvider::createID())
	{
		load(vertexPath, fragmentPath);
	}

	void Shader::load(const GLchar* vertexPath, const GLchar* fragmentPath)
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
		vertex = glCreateShader(GL_VERTEX_SHADER); GLERR
		glShaderSource(vertex, 1, &vShaderCode, NULL);GLERR
		glCompileShader(vertex); GLERR
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); GLERR
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog); GLERR
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER); GLERR
		glShaderSource(fragment, 1, &fShaderCode, NULL); GLERR
		glCompileShader(fragment); GLERR
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); GLERR
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog); GLERR
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader Program
		this->Program = glCreateProgram(); GLERR
		glAttachShader(this->Program, vertex); GLERR
		glAttachShader(this->Program, fragment); GLERR
		glLinkProgram(this->Program); GLERR
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success); GLERR
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog); GLERR
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDetachShader(this->Program, vertex); GLERR
		glDetachShader(this->Program, fragment); GLERR
		glDeleteShader(vertex); GLERR
		glDeleteShader(fragment); GLERR
	}
	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program); GLERR
	}

	bool isActive()
	{
		GLint progName = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &progName); GLERR
		if (progName != this->Program)
			return false;
		return true;
	}

private:
	GLint getUniformLocation(std::string name)
	{
		return glGetUniformLocation(this->Program, name.c_str()); GLERR
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
	{
		glUseProgram(this->Program); GLERR
	}
	glUniform1f(loc, value); GLERR

}

template<> 
inline void Shader::setUniform(const std::string name, std::vector<GLfloat> values)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
	{
		glUseProgram(this->Program); GLERR
	}
	auto size = values.size();
	switch (size)
	{
		case 2:
			glUniform1fv(loc, 2, values.data()); GLERR
			break;
		case 3:
			glUniform1fv(loc, 3, values.data()); GLERR
			break;
		case 4:
			glUniform1fv(loc, 4, values.data()); GLERR
			break;
		default:
			std::cout << "VECTOR TOO LARGE OR < 2 VALUES";
	}
}

template<>
inline void Shader::setUniform(const std::string name,glm::vec2 values)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
	{
		glUseProgram(this->Program); GLERR
	}

	glUniform2fv(loc, 1, glm::value_ptr(values)); GLERR
}

template<>
inline void Shader::setUniform(const std::string name, glm::vec3 values)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
	{
		glUseProgram(this->Program); GLERR
	}

	glUniform3fv(loc, 1, glm::value_ptr(values)); GLERR
}

template<>
inline void Shader::setUniform(const std::string name, glm::vec4 values)
{	
	GLint loc = getUniformLocation(name);	
	if (!isActive())
	{
		glUseProgram(this->Program); GLERR
	}
	glUniform4fv(loc, 1, glm::value_ptr(values)); GLERR
}

template<>
inline void Shader::setUniform(const std::string name, const GLint value)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
	{
		glUseProgram(this->Program); GLERR
	}
	glUniform1i(loc, value); GLERR
}

template<>
inline void Shader::setUniform(const std::string name, const GLuint value)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
	{
		glUseProgram(this->Program); GLERR
	}
	glUniform1ui(loc, value); GLERR
}

template<>
inline void Shader::setUniform(const std::string name, glm::mat4 value, const GLboolean transpose)
{
	GLint loc = getUniformLocation(name);
	if (!isActive())
	{
		glUseProgram(this->Program); GLERR
	}
	glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(value)); GLERR
}