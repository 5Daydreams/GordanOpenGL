#include "shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw (errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLCall( GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GLCall(glShaderSource(vertexShader, 1, &vertexSource, NULL));
	GLCall(glCompileShader(vertexShader));
	compileErrors(vertexShader, "VERTEX");

	GLCall(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fragmentShader, 1, &fragmentSource, NULL));
	GLCall(glCompileShader(fragmentShader));
	compileErrors(fragmentShader, "FRAGMENT");

	GLCall(ID = glCreateProgram());
	GLCall(glAttachShader(ID, vertexShader));
	GLCall(glAttachShader(ID, fragmentShader));
	GLCall(glLinkProgram(ID));
	compileErrors(ID, "PROGRAM");

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));
}

void Shader::Activate()
{
	GLCall(glUseProgram(ID));
}

void Shader::Delete()
{
	GLCall(glDeleteProgram(ID));
}

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled));
		if (hasCompiled == GL_FALSE)
		{
			GLCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		GLCall(glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled));
		if (hasCompiled == GL_FALSE)
		{
			GLCall(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}