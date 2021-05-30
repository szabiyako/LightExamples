#include "Shader.h"

#include <GL/glew.h>
#include "ErrorHandler.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string & filepath)
	: m_filePath(filepath), m_id(0)
{
	ShaderProgramSource source = parseShader(filepath);
	m_id = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_id));
}


ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		//allocate pointer on stack, not on dram
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile ";
		std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::cout << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);

		return 0;
	}

	return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int programm = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(programm, vs);
	glAttachShader(programm, fs);
	glLinkProgram(programm);
	glValidateProgram(programm);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return programm;
}

void Shader::bind() const
{
	GLCall(glUseProgram(m_id));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

int Shader::getUniformLocation(const std::string & name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_id, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn`t exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}

//UNIFORMS

void Shader::setUniform1i(const std::string & name, int value)
{
	GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string & name, float value)
{
	GLCall(glUniform1f(getUniformLocation(name), value));
}


void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformVec2f(const std::string & name, glm::vec2 vec)
{
	GLCall(glUniform2f(getUniformLocation(name), vec[0], vec[1]));
}

void Shader::setUniformVec3f(const std::string & name, glm::vec3 vec)
{
	GLCall(glUniform3f(getUniformLocation(name), vec[0], vec[1], vec[2]));
}

void Shader::setUniformVec4f(const std::string & name, glm::vec4 vec)
{
	GLCall(glUniform4f(getUniformLocation(name), vec[0], vec[1], vec[2], vec[3]));
}

void Shader::setUniformMatrix4f(const std::string & name, glm::mat4 mat4)
{
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat4[0][0]));
}
