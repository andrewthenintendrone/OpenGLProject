#include "Shader.h"
#include <iostream>

// constructor generates the shader on the fly
Shader::Shader(const char* vertexPath,
	const char* fragmentPath,
	const char* geometryPath,
	const char* tessCPath,
	const char* tessEPath)
{
	// create ifstreams and strings for reading in shaders from files
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::string tessCCode;
	std::string tessECode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	std::ifstream tessCFile;
	std::ifstream tessEFile;

	// enable exceptions on ifstreams
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tessCFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tessEFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		std::stringstream vShaderStream, fShaderStream, gShaderStream, tessCShaderStream, tessEShaderStream;

		// open files
		if (vertexPath)
		{
			vShaderFile.open(vertexPath);
			vShaderStream << vShaderFile.rdbuf();
			vShaderFile.close();
			vertexCode = vShaderStream.str();
		}
		if (fragmentPath)
		{
			fShaderFile.open(fragmentPath);
			fShaderStream << fShaderFile.rdbuf();
			fShaderFile.close();
			fragmentCode = fShaderStream.str();
		}
		if (geometryPath)
		{
			gShaderFile.open(geometryPath);
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
		if (tessCPath)
		{
			tessCFile.open(tessCPath);
			tessCShaderStream << tessCFile.rdbuf();
			tessCFile.close();
			tessCCode = tessCShaderStream.str();
		}
		if (tessEPath)
		{
			tessEFile.open(tessEPath);
			tessEShaderStream << tessEFile.rdbuf();
			tessEFile.close();
			tessECode = tessEShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Error reading shader file" << std::endl;
	}

	// compile shaders
	unsigned int vertex, fragment, geometry, tessC, tessE;
	ID = glCreateProgram();

	if (vertexPath)
	{
		const char* vShaderCode = vertexCode.c_str();
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		glAttachShader(ID, vertex);
	}
	if (fragmentPath)
	{
		const char* fShaderCode = fragmentCode.c_str();
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		glAttachShader(ID, fragment);
	}
	if (geometryPath)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
		glAttachShader(ID, geometry);
	}
	if (tessCPath)
	{
		const char* tessCShaderCode = tessCCode.c_str();
		tessC = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tessC, 1, &tessCShaderCode, NULL);
		glCompileShader(tessC);
		checkCompileErrors(tessC, "TESSELLATION_CONTROL");
		glAttachShader(ID, tessC);
	}
	if (tessEPath)
	{
		const char* tessEShaderCode = tessECode.c_str();
		tessE = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tessE, 1, &tessEShaderCode, NULL);
		glCompileShader(tessE);
		checkCompileErrors(tessE, "TESSELLATION_EVALUATION");
		glAttachShader(ID, tessE);
	}

	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessery
	if (vertexPath)
	{
		glDeleteShader(vertex);
	}
	if (fragmentPath)
	{
		glDeleteShader(fragment);
	}
	if (geometryPath)
	{
		glDeleteShader(geometry);
	}
	if (tessCPath)
	{
		glDeleteShader(tessC);
	}
	if (tessEPath)
	{
		glDeleteShader(tessE);
	}
}

// activate this Shader
void Shader::bind()
{
	glUseProgram(ID);
}

// set a boolean in the shader
const void Shader::setBool(const std::string& name, bool value)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform1i(uniformLocation, (int)value);
	}
}

// set an int in the shader
const void Shader::setInt(const std::string& name, int value)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform1i(uniformLocation, value);
	}
}

// set a float value in the shader
const void Shader::setFloat(const std::string& name, float value)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform1f(uniformLocation, value);
	}
}

// set a vector 2 in the shader
const void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform2fv(uniformLocation, 1, &value[0]);
	}
}

// set a vector2 in the shader
const void Shader::setVec2(const std::string& name, float x, float y)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform2f(uniformLocation, x, y);
	}
}

// set a vector3 in the shader
const void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform3fv(uniformLocation, 1, &value[0]);
	}
}

// set a vector4 in the shader
const void Shader::setVec3(const std::string& name, float x, float y, float z)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform3f(uniformLocation, x, y, z);
	}
}

// set a vector4 in the shader
const void Shader::setVec4(const std::string& name, const glm::vec4& value)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform4fv(uniformLocation, 1, &value[0]);
	}
}

// set a vector4 in the shader
const void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniform4f(uniformLocation, x, y, z, w);
	}
}

// set a matrix2 in the shader
const void Shader::setMat2(const std::string& name, const glm::mat2& mat)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
	}
}

// set a matrix3 in the shader
const void Shader::setMat3(const std::string& name, const glm::mat3& mat)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
	}
}

// set a matrix4 in the shader
const void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());

	if (uniformLocation >= 0)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
	}
}

// outputs any shader compile errors to the console
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Error compiling " << type << " Shader\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Error linking " << type << " Program\n" << infoLog << std::endl;
		}
	}
}
