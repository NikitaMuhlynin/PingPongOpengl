#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

ShaderProgram::~ShaderProgram() {
	if (m_programId != 0)
		glDeleteProgram(m_programId);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: m_programId(other.m_programId) {
	other.m_programId = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
	if (this != &other) {
		if (m_programId != 0)
			glDeleteProgram(m_programId);

		m_programId = other.m_programId;
		other.m_programId = 0;
	}

	return *this;
}

std::string ShaderProgram::readTextFile(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Не удалось открыть файл шейдера: " + path);

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

GLuint ShaderProgram::compileShader(GLenum shaderType, const std::string& source) {
	GLuint shader = glCreateShader(shaderType);
	const char* src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetShaderInfoLog(shader, logLength, nullptr, &log[0]);

		glDeleteShader(shader);
		throw std::runtime_error("Ошибка компиляции\n" + log);
	}

	return shader;
}

bool ShaderProgram::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexSource = readTextFile(vertexPath);
	std::string fragmentSource = readTextFile(fragmentPath);

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		GLint logLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetProgramInfoLog(program, logLength, nullptr, &log[0]);

		glDeleteProgram(program);
		throw std::runtime_error("Ошибка линковки\n" + log);
	}

	if (m_programId != 0) {
		glDeleteProgram(m_programId);
	}

	m_programId = program;
	return true;
}

void ShaderProgram::use() const {
	glUseProgram(m_programId);
}

void ShaderProgram::setVec3(const std::string& uniformName, float x, float y, float z) const {
	GLint location = glGetUniformLocation(m_programId, uniformName.c_str());
	glUniform3f(location, x, y, z);
}