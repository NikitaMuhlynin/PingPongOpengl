#pragma once

#include <string>
#include <glad/glad.h>

class ShaderProgram {
public:
	ShaderProgram() = default;
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
	void use() const;

	void setVec3(const std::string& uniformName, float x, float y, float z) const;

	GLuint id() const { return m_programId; }

private:
	GLuint m_programId = 0;

	static std::string readTextFile(const std::string& path);
	static GLuint compileShader(GLenum shaderType, const std::string& source);
};