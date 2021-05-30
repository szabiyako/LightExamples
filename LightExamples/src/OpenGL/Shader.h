#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

#include "Utils/NonCopyable.h"

class Shader : public NonCopyable
{
private:
	unsigned int m_id;
	std::string m_filePath;

	// caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	//Set uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformVec2f(const std::string& name, glm::vec2 vec);
	void setUniformVec3f(const std::string& name, glm::vec3 vec);
	void setUniformVec4f(const std::string& name, glm::vec4 vec);
	void setUniformMatrix4f(const std::string& name, glm::mat4 mat4);
private:
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int getUniformLocation(const std::string &name);

};

