#pragma once
#include<string>
#include<GL/glew.h>
#include<vector>
#include"Error.h"
#include<fstream>
using namespace std;

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();
	void compileShaders(const string& vertexShaderPath, const string& fragmentShaderPath);
	void addAttribute(const string& atttributeName);
	void linkShaders();
	void use();
	void unuse();
	void check_errors();
	GLint getUniformLocation(const std::string& uniformName);
private:
	void compileShader(const string& filePath, GLuint id);
	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
	int _numAttributes;
	

};

