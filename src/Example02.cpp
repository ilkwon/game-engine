#include "Example02.h"

#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

//-----------------------------------------------------------------------------
Example02::Example02()
{
	mTitle = "Example02_Triangle";
}

//-----------------------------------------------------------------------------
Example02::~Example02()
{
}

//-----------------------------------------------------------------------------
void Example02::CreateDefaultShader()
{
	// 버텍스 셰이더 코드.
	std::string vertexCode = R"(
		#version 460 core
		layout (location = 0) in vec3 aPos;
		void main()
		{
				gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)";
	// 프래그먼트 셰이더 코드.
	std::string fragmentCode = R"(
		#version 460 core
		out vec4 FragColor;
		void main()
		{
				FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		}
		)";

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	// 버텍스 셰이더 컴파일
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckShaderCompileErrors(vertex, "VERTEX");	// 컴파일 에러 체크

	// 프래그먼트 셰이더 컴파일
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckShaderCompileErrors(fragment, "FRAGMENT");	// 컴파일 에러 체크

	// 셰이더 프로그램 생성 및 링크
	mDefaultShaderID = glCreateProgram();
	glAttachShader(mDefaultShaderID, vertex);
	glAttachShader(mDefaultShaderID, fragment);
	glLinkProgram(mDefaultShaderID);
	CheckShaderCompileErrors(mDefaultShaderID, "PROGRAM");	// 링크 에러 체크

	// 컴파일과 링크(등록)가 끝난 셰이더는 더 이상 필요 없으므로 삭제
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

//-----------------------------------------------------------------------------
void Example02::CheckShaderCompileErrors(unsigned int shader, std::string type)
{
	GLint success{};
	GLchar errorLog[1024]{};

	if ("PROGRAM" == type) {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, errorLog);
			std::cerr << "ERROR::SHADER_PROGRAM_LINKING_ERROR of type: " << type << "\n"
				<< errorLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else {				
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, errorLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
				<< errorLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

//-----------------------------------------------------------------------------
void Example02::DeleteDefaultShader()
{
	glDeleteProgram(mDefaultShaderID);
}

//-----------------------------------------------------------------------------
void Example02::CreateTriangle()
{
	/*
	*          (0, 1, 0)
	*          (index:0)
	*              *
	*             * *
	*            *   *
	*           *     *
	*          *   +   *
	*         * (center)*
	*        *  (0,0,0)  *
	*       *             *
	*      *****************
	* (-1,-1,0)         (1,-1,0)
	* (index:1)         (index:2)
	*/

	// 삼각형의 버텍스 좌표들.
	mVertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));   // index: 0
	mVertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f)); // index: 1
	mVertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));  // index: 2
	// 삼각형의 인덱스 배열.
	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);

}

//-----------------------------------------------------------------------------
void Example02::CreateVertexBuffer()
{
	// 버텍스 버퍼와 인덱스 버퍼를 보관할 "vertex array" 오브젝트 생성.
	glGenVertexArrays(1, &mVertexArrayObjectId);
	// 버텍스 버퍼 생성
	glGenBuffers(1, &mVertexBufferObjectId);
	// element(인덱스) 버퍼 생성
	glGenBuffers(1, &mElementBufferObjectId);
	
	// VAO 바인딩.
	glBindVertexArray(mVertexArrayObjectId);
	{
		// VBO 바인딩 및 버텍스 데이터 업로드.
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectId);					
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), mVertices.data(), GL_STATIC_DRAW);
		
		// EBO에 인덱스 데이터 업로드.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferObjectId);					
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);
		
		// 버텍스 속성 포인터 설정.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		// 바인딩 해제.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

//-----------------------------------------------------------------------------
void Example02::Initialize()
{
	CreateDefaultShader();
	CreateTriangle();
	CreateVertexBuffer();
}

//-----------------------------------------------------------------------------
void Example02::Render()
{
	// 렌더링에 적용할 셰이더 프로그램 설정.
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
void Example02::CleanUp()
{
	DeleteVertexBuffer();
	DeleteDefaultShader();
}

//-----------------------------------------------------------------------------
void Example02::DeleteVertexBuffer()
{
	glDeleteVertexArrays(1, &mVertexArrayObjectId);
	glDeleteBuffers(1, &mVertexBufferObjectId);
	glDeleteBuffers(1, &mElementBufferObjectId);
}

//-----------------------------------------------------------------------------