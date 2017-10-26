#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <shader.hpp>
#include "spdlog/spdlog.h"
#include "GL/glew.h"

auto shaderLogger = spdlog::stdout_color_mt("shader");

const char* loadFile(const char* filePath)
{
	const char * shaderFileBuffer = NULL;
	std::ifstream inSdrFileStream(filePath);
	if(inSdrFileStream)
	{
		inSdrFileStream.seekg(0, inSdrFileStream.end);
		unsigned long fileLength = (unsigned long) inSdrFileStream.tellg() + 1;
		inSdrFileStream.seekg(0, inSdrFileStream.beg);
		std::cout << "Shader File: Reading " << fileLength << " chars...\n";
		char * inputFileBuffer = new char[fileLength];
		memset(inputFileBuffer, 0, fileLength);
		inSdrFileStream.read(inputFileBuffer, fileLength);
		inputFileBuffer[fileLength-1] = 0;
		if(inputFileBuffer)
		{
			std::cout << "... Read successfully.\n\n";
			std::cout << "---------------------------------\n";
			std::cout << inputFileBuffer << std::endl;
			std::cout << "---------------------------------\n";
			std::cout << std::endl;
			inSdrFileStream.close();
		}
		else
		{
			std::cout << "... Error: Only " << inSdrFileStream.gcount() << " could be read!\n";
			inSdrFileStream.close();
			delete [] inputFileBuffer;
			return NULL;
		}
		shaderFileBuffer = inputFileBuffer;
		inputFileBuffer = NULL;
	}
	else
	{
		std::cout << "Shader File: Error. Not found!" << std::endl;
		return NULL;
	}
	return shaderFileBuffer;
}

Shader::Shader(char const * vertexShaderFile, char const * fragmentShaderFile)
{
  vertexShaderCode = loadFile(vertexShaderFile);
  fragmentShaderCode = loadFile(fragmentShaderFile);
}

bool shaderStateOk(GLint shaderId)
{
  GLint isCompiled = 0;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
  	GLint maxLength = 0;
  	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
  	std::vector<GLchar> errorLog(maxLength);
  	glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
    for (auto i = errorLog.begin(); i != errorLog.end(); ++i)
      std::cout << *i;
  	glDeleteShader(shaderId);
  	return false;
  }
  return true;
}

bool programStateOk(GLint ProgramID)
{
  GLint result = GL_FALSE;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
  if(result == GL_FALSE)
  {
    GLint maxLength;
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &maxLength);
  	std::vector<GLchar> errorLog(maxLength);
  	glGetProgramInfoLog(ProgramID, maxLength, &maxLength, &errorLog[0]);
    for (auto i = errorLog.begin(); i != errorLog.end(); ++i)
      std::cout << *i;
    return false;
  }
  return true;
}

bool Shader::init()
{
  shaderLogger->info("Initializing shader program");

  shaderLogger->info("Creating vertex shader");
  this->VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->VertexShaderId, 1, &(this->vertexShaderCode), 0);
	glCompileShader(this->VertexShaderId);

  if(!shaderStateOk(this->VertexShaderId)){
    shaderLogger->info("Error compiling vertex shader");
    return false;
  }
  shaderLogger->info("Compiled vertex shader");


  shaderLogger->info("Creating fragment shader");
	this->FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->FragmentShaderId, 1, &(this->fragmentShaderCode), 0);
	glCompileShader(this->FragmentShaderId);

  if(!shaderStateOk(this->FragmentShaderId)){
    shaderLogger->info("Error compiling fragment shader");
    return false;
  }
  shaderLogger->info("Compiled fragment shader");

  shaderLogger->info("Creating shader program");
	this->ProgramId = glCreateProgram();
	glAttachShader(this->ProgramId, this->VertexShaderId);
	glAttachShader(this->ProgramId, this->FragmentShaderId);

  shaderLogger->info("Binding attributes");
	glBindAttribLocation(this->ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(this->ProgramId, COLORS, "in_Color");
  shaderLogger->info("Linking shader program");
	glLinkProgram(this->ProgramId);

  if(!programStateOk(this->ProgramId)){
    shaderLogger->error("Error linking shader program");
    return false;
  }
  shaderLogger->info("Linked shader program");

	shaderLogger->info("Binding uniforms");
	this->uProjectionMat = glGetUniformLocation(this->ProgramId, "projectionMat");
	this->uViewMat = glGetUniformLocation(this->ProgramId, "viewMat");
	this->uModelMat = glGetUniformLocation(this->ProgramId, "modelMat");	
	
  this->uColor = glGetUniformLocation(this->ProgramId, "uColor");

  return true;
}

bool Shader::shutdown()
{
  glUseProgram(0);
	glDetachShader(this->ProgramId, this->VertexShaderId);
	glDetachShader(this->ProgramId, this->FragmentShaderId);

	glDeleteShader(this->FragmentShaderId);
	glDeleteShader(this->VertexShaderId);
	glDeleteProgram(this->ProgramId);
  return true;
}
