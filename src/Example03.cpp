#include "Example03.h"

#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

//-----------------------------------------------------------------------------
Example03::Example03()
{
	mTitle = "Example03_Triangle_Color";
}

//-----------------------------------------------------------------------------
Example03::~Example03()
{
}
//-----------------------------------------------------------------------------
void Example03::CreateDefaultShader()
{
	// Vertex Shader 소스 코드
	std::string vertexCode = R"(
		#version 460 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;
		out vec3 vertexColor;
		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
			vertexColor = aColor;
		}
		)";
	// Fragment Shader 소스 코드
	std::string fragmentCode = R"(
		#version 460 core
		in vec3 vertexColor;
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(vertexColor, 1.0f);
		}
		)";

	const char* VERTEX_SHADER_CODE = vertexCode.c_str();
	const char* FRAGMENT_SHADER_CODE = fragmentCode.c_str();

	unsigned int vertex{};
	unsigned int fragment{};

	// 버텍스 셰이더 컴파일을 수행한다.
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &VERTEX_SHADER_CODE, NULL);
	glCompileShader(vertex);
	// 컴파일 에러가 발생했는지 체크 한다.
	CheckShaderCompileErrors(vertex, "VERTEX");

	// 프래그먼트 셰이더 컴파일을 수행한다.
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &FRAGMENT_SHADER_CODE, NULL);
	glCompileShader(fragment);
	// 컴파일 에러가 발생했는지 체크 한다.
	CheckShaderCompileErrors(fragment, "FRAGMENT");

	// OpenGL에 셰이더 프로그램을 등록한다.
	mDefaultShaderID = glCreateProgram();
	glAttachShader(mDefaultShaderID, vertex);
	glAttachShader(mDefaultShaderID, fragment);

	glLinkProgram(mDefaultShaderID);
	// 컴파일 에러가 발생했는지 체크 한다.
	CheckShaderCompileErrors(mDefaultShaderID, "PROGRAM");

	// 컴파일과 프로그램 등록이 모두 완료되었으면 더이상 갖고 있을 필요가 없으므로 자원을 해제 한다.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
//-----------------------------------------------------------------------------
void Example03::CheckShaderCompileErrors(unsigned int shader, std::string type)
{
	GLint success{};
	GLchar errorLog[1024]{};

	if ("PROGRAM" == type)
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) 
		{
			glGetProgramInfoLog(shader, 1024, NULL, errorLog);
			std::cout << "[error] shader program linking error, type" << type << std::endl;
			std::cout << "log : " << errorLog << std::endl;
		}
		else {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, errorLog);
				std::cout << "[error] shader compilation error. type" << type << std::endl;
				std::cout << "log : " << errorLog << std::endl;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void Example03::DeleteDefaultShader() 
{
	glDeleteProgram(mDefaultShaderID);
}
//-----------------------------------------------------------------------------
void Example03::CreateTriangle() 
{
	glm::vec3 RED = glm::vec3(1, 0, 0);
	glm::vec3 GREEN = glm::vec3(0, 1, 0);
	glm::vec3 BLUE = glm::vec3(0, 0, 1);
	mVertices.push_back(Vertex(glm::vec3(0.0f, 1.0f, 0.0f), RED));       // center top.
	mVertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), GREEN));     // left bottom.
	mVertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), BLUE));      // right bottom.

	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);
}
//-----------------------------------------------------------------------------
void Example03::CreateVertexBuffer() 
{
	// 버텍스 버퍼와 인덱스 버퍼를 보관할 "vertex array" 오브젝트를 생성한다.
	glGenVertexArrays(1, &mVertexArrayObjectId);
	// Create vertex buffer.
	glGenBuffers(1, &mVertexBufferObjectId);
	// Create element buffer.(index buffer)
	glGenBuffers(1, &mElementBufferObjectId);

	// Bind VAO.
	glBindVertexArray(mVertexArrayObjectId);
	{
		// 버텍스 데이터를 버텍스 버퍼에 올린다.
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectId);
		{
			glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
		}

		// 인덱스 데이터를 인덱스 버퍼(element array buffer)에 올린다.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferObjectId);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);
		}

		// 버텍스 속성(attribute)을 설정한다.
		glEnableVertexAttribArray(0);
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
		}

		glEnableVertexAttribArray(1);
		{
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mColor));
		}
	}

	// Unbind.
	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------

void Example03::Initialize()
{
	CreateDefaultShader();
	CreateTriangle();
	CreateVertexBuffer();
}
//-----------------------------------------------------------------------------
void Example03::Render()
{
	// 렌더링할 때 사용할 셰이더 프로그램을 활성화 한다.
	glUseProgram(mDefaultShaderID);
	// VAO 바인딩.
	glBindVertexArray(mVertexArrayObjectId);
	{
		// 삼각형 렌더링.
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
void Example03::CleanUp()
{
	DeleteVertexBuffer();
	DeleteDefaultShader();
}
//-----------------------------------------------------------------------------
void Example03::DeleteVertexBuffer()
{
	glDeleteBuffers(1, &mElementBufferObjectId);
	glDeleteBuffers(1, &mVertexBufferObjectId);
	glDeleteVertexArrays(1, &mVertexArrayObjectId);
}
//-----------------------------------------------------------------------------
