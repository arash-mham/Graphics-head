#include "GLSLProgram.h"



GLSLProgram::GLSLProgram()
{
	_programID = 0;
	_vertexShaderID = 0;
	_fragmentShaderID = 0;
	_numAttributes=0;
}
GLSLProgram::~GLSLProgram()
{
}
void GLSLProgram::compileShaders(const string& vertexShaderPath, const string& fragmentShaderPath)
{
	//Get a program object and compile shaders.
	_programID = glCreateProgram();

	//An error object in case we fail to compile or load the shaders
	Error er;
	/*Specifies the type of shader to be created. Must be one of GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER. Returns 0 if makes an error*/
	_vertexShaderID= glCreateShader(GL_VERTEX_SHADER);	
	if (_vertexShaderID==0)
	{
		er.fatalError("vertex shader failed to be created");
	}
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID==0)
	{
		er.fatalError("fragment shader failed to be created");
	}
	
	//we need to read the code from the shaders
	compileShader(vertexShaderPath, _vertexShaderID);
	compileShader(fragmentShaderPath, _fragmentShaderID);
	
	
	
}
void GLSLProgram::compileShader(const string& filePath, GLuint id)
{
	
	
	Error er;

	// we need to read the code from the shaders
	fstream ShaderFile(filePath);
	if (ShaderFile.fail())
	{
		er.fatalError("Failed to open shader");
	}


	//we need to read the netire file in one string to be able to send it to GPU
	string fileContents = "";
	string line;
	while (getline(ShaderFile, line))
	{
		fileContents += line + "\n";
	}
	ShaderFile.close();


	/*glShaderSource — Replaces the source code in a shader object
	void glShaderSource(	GLuint shader,	GLsizei count,const GLchar **string,const GLint *length);
	shader:Specifies the handle of the shader object whose source code is to be replaced.
	count:specifies the number of elements in the string and length arrays.
	string:Specifies an array of pointers to strings containing the source code to be loaded into the shader.
	length:		Specifies an array of string lengths.*/
	const char* contentsptr = fileContents.c_str();
	glShaderSource(id, 1, &contentsptr, nullptr);
	//finally we need to compile our shader
	glCompileShader(id);

	//error checking

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glDeleteShader(id);
		er.fatalError(filePath+" failed to compile");
	}
}
void GLSLProgram::linkShaders() {
	Error er;
	
	//Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		//We don't need the program anymore.
		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
		er.fatalError("Shaders failed to link!");
	}

	//Always detach shaders after a successful link.
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}
void GLSLProgram::addAttribute(const string& atttributeName)
{
	/* associate a generic vertex attribute index with a named attribute variable

	void glBindAttribLocation(	GLuint program,	GLuint index,const GLchar *name);

	program	Specifies the handle of the program object in which the association is to be made.

	index	Specifies the index of the generic vertex attribute to be bound.

	name	Specifies a null terminated string containing the name of the vertex shader attribute variable to which index is to be bound.

	*/
	glBindAttribLocation(_programID, _numAttributes++, atttributeName.c_str());// note that ++ happens after the line of code
}
void GLSLProgram::use()
{
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}
void GLSLProgram::unuse()
{
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}
GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
{
	Error Er;
	GLint location=glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
		Er.fatalError("Uniform" + uniformName + "not found in the shader");
	return location;
}
void GLSLProgram::check_errors()
{
	GLint maxLength = 0;
	GLint maxLength2 =0;
	glGetShaderiv(_fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);
	// The maxLength includes the NULL character
	std::vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(_fragmentShaderID, maxLength, &maxLength, &errorLog[0]);
	glGetShaderiv(_vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength2);
	// The maxLength includes the NULL character
	std::vector<GLchar> errorLog2(maxLength2);
	glGetShaderInfoLog(_vertexShaderID, maxLength2, &maxLength2, &errorLog2[0]);
	
}
