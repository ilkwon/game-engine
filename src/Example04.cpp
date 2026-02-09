#include "Example04.h"

#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


Example04::Example04()
{
	mTitle = "Example04_Texture";
}

Example04::~Example04()
{
}

//---------------------------------------------------------------------------
void Example04::RunWidthParam(UvType uvType, int wrapType, int filterType) 
{
	mUvType = uvType;
	mWrapType = wrapType;
	mFilterType = filterType;

	ExampleBase::Run();
}

//---------------------------------------------------------------------------
// 셰이더 관련.
void Example04::CreateDefaultShader() 
{
	// 셰이더 소스 코드
	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;
		layout (location = 2) in vec2 aTexCoord;
		out vec3 ourColor;
		out vec2 TexCoord;
		void main()
		{
			gl_Position = vec4(aPos, 1.0);
			ourColor = aColor;
			TexCoord = aTexCoord;
		}
	)";

	// 프래그먼트 셰이더 소스 코드
	const char* fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec3 ourColor;
		in vec2 TexCoord;
		uniform sampler2D texture1;
		uniform sampler2D texture2;
		void main()
		{
			FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
		}
	)";

	const char* VERTEX_SHADER_CODE = vertexShaderSource;
	const char* FRAGMENT_SHADER_CODE = fragmentShaderSource;

	unsigned int vertex{};
	unsigned int fragment{};
	// vertex shader 생성 및 컴파일
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &VERTEX_SHADER_CODE, NULL);
	glCompileShader(vertex);
	CheckShaderCompileErrors(vertex, "VERTEX");	// 컴파일 오류 체크
	
	// fragment shader 생성 및 컴파일
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &FRAGMENT_SHADER_CODE, NULL);
	glCompileShader(fragment);
	CheckShaderCompileErrors(fragment, "FRAGMENT"); // 컴파일 오류 체크

	// shader program 생성 및 링크
	mDefaultShaderID = glCreateProgram();				
	glAttachShader(mDefaultShaderID, vertex);		
	glAttachShader(mDefaultShaderID, fragment); 
	
	glLinkProgram(mDefaultShaderID);	
	CheckShaderCompileErrors(mDefaultShaderID, "PROGRAM"); 

	// 셰이더 객체는 더 이상 필요 없으므로 삭제
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
//---------------------------------------------------------------------------
void Example04::CheckShaderCompileErrors(unsigned int shader, std::string type) 
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " 
				<< std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " 
				<< std::endl;
		}
	}
}

//---------------------------------------------------------------------------
void Example04::DeleteDefaultShader() 
{
	glDeleteProgram(mDefaultShaderID);
}

//---------------------------------------------------------------------------
// 삼각형 렌더링 관련.
void Example04::CreateRectangle() 
{
	glm::vec2 uvLeftTop{ 0};
	glm::vec2 uvRightTop{ 0 };
	glm::vec2 uvRightBottom{ 0 };
	glm::vec2 uvLeftBottom{ 0 };

	switch (mUvType) {
		case UvType::Fit:
		// 기본 UV 좌표 사용
			uvLeftTop = glm::vec2{ 0.0f, 1.0f };
			uvRightTop = glm::vec2{ 1.0f, 1.0f };
			uvRightBottom = glm::vec2{ 1.0f, 0.0f };
			uvLeftBottom = glm::vec2{ 0.0f, 0.0f };
			break;
		case UvType::Smaller:
			// UV 좌표를 0.5배 축소
			uvLeftTop = glm::vec2{ 0.25f, 0.75f };
			uvRightTop = glm::vec2{ 0.75f, 0.75f };
			uvRightBottom = glm::vec2{ 0.75f, 0.25f };
			uvLeftBottom = glm::vec2{ 0.25f, 0.25f };

			break;
		case UvType::Bigger:
			// UV 좌표를 2배 확대
			uvLeftTop = glm::vec2{ -0.5f, 1.5f };
			uvRightTop = glm::vec2{ 1.5f, 1.5f };
			uvRightBottom = glm::vec2{ 1.5f, -0.5f };
			uvLeftBottom = glm::vec2{ -0.5f, -0.5f };
			break;
	}

	float scale = 0.8f;
	mVertices.push_back(VertexUV(scale * glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1, 0, 0), uvLeftTop));     // left top
	mVertices.push_back(VertexUV(scale * glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0, 1, 0), uvLeftBottom));    // left bottom
	mVertices.push_back(VertexUV(scale * glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0, 0, 1), uvRightBottom));     // right bottom
	mVertices.push_back(VertexUV(scale * glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0, 0, 1), uvRightTop));      // right top

	mIndices.push_back(0);  // first vertex.
	mIndices.push_back(1);  // second vertex.
	mIndices.push_back(2);  // third vertex.

	mIndices.push_back(0);  // first vertex.
	mIndices.push_back(2);  // second vertex.
	mIndices.push_back(3);  // third vertex.
}

//---------------------------------------------------------------------------
void Example04::CreateVertexBuffer() 
{
	glGenVertexArrays(1, &mVertexArrayObjectId); // 정점 배열 객체 생성
	glGenBuffers(1, &mVertexBufferObjectId);	// 정점 버퍼 객체 생성
	glGenBuffers(1, &mElementBufferObjectId); // 요소 버퍼 객체 생성
	glBindVertexArray(mVertexArrayObjectId); // 정점 배열 객체 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectId); // 정점 버퍼 객체 바인딩
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(VertexUV), mVertices.data(), GL_STATIC_DRAW); // 정점 데이터 업로드
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferObjectId); // 요소 버퍼 객체 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW); // 요소 데이터 업로드
	// 정점 속성 설정
	// 위치 속성
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, mPosition));
	glEnableVertexAttribArray(0);
	// 색상 속성
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, mColor));
	glEnableVertexAttribArray(1);
	// 텍스처 좌표 속성
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, mUv));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 정점 버퍼 객체 바인딩 해제
	glBindVertexArray(0); // 정점 배열 객체 바인딩 해제
}

//---------------------------------------------------------------------------
void Example04::DeleteVertexBuffer() {
	glDeleteVertexArrays(1, &mVertexArrayObjectId); // 정점 배열 객체 삭제
	glDeleteBuffers(1, &mVertexBufferObjectId);	// 정점 버퍼 객체 삭제
	glDeleteBuffers(1, &mElementBufferObjectId); // 요소 버퍼 객체 삭제
}

//---------------------------------------------------------------------------
unsigned int Example04::LoadTexture(const std::string& path) 
{
	unsigned int textureID{ };
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// texture wrap
	if (mWrapType == GL_CLAMP_TO_BORDER) {
		const float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);	// 테두리 색상 지정
	}

	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapType);		// S 방향 래핑 방식 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapType); 	// T 방향 래핑 방식 설정
	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilterType); // 축소 필터링 방식 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilterType); // 확대 필터링 방식 설정

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // 이미지 수직 뒤집기 설정
	unsigned char* imgdata = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (imgdata) {
		switch (nrChannels) {
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, imgdata);	// 흑백 이미지
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgdata);	// 일반적인 RGB 이미지
			break;
		case 4: 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata); // 알파 채널이 있는 RGBA 이미지
			break;
		default:
			std::cout << "Unsupported number of channels: " << nrChannels << std::endl;
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(imgdata);
	} else {
		std::cout << "Failed to load texture at path: " << path << std::endl;
	}
	return textureID;
}

//---------------------------------------------------------------------------
void Example04::Initialize() 
{
	CreateDefaultShader();
	CreateRectangle();
	CreateVertexBuffer();

	mTextureId0 = LoadTexture("../resources/opengl_texture.png");
	mTextureId1 = LoadTexture("../resources/circle_mask.png");
}

//---------------------------------------------------------------------------
void Example04::Render() 
{
	// 렌더링에 적용할 셰이더 프로그램 사용
	glUseProgram(mDefaultShaderID);
	// 텍스처 유닛 설정 및 바인딩
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureId0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextureId1);
	// 셰이더의 샘플러 유니폼에 텍스처 유닛 인덱스 설정
	glUniform1i(glGetUniformLocation(mDefaultShaderID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(mDefaultShaderID, "texture2"), 1);
	
	// 정점 배열 객체 바인딩
	glBindVertexArray(mVertexArrayObjectId);
	// 삼각형 렌더링
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0); // 정점 배열 객체 바인딩 해제
}

//---------------------------------------------------------------------------
void Example04::CleanUp() {
	DeleteVertexBuffer();
	DeleteDefaultShader();
}

//---------------------------------------------------------------------------

