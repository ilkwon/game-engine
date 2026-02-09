#include "ExampleBase.h"

#include <iostream>
#include <string>
#include <thread>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

//-----------------------------------------------------------------------------
ExampleBase::ExampleBase() {
	mTitle = "Example01_Window";
}
//-----------------------------------------------------------------------------
ExampleBase::~ExampleBase() {

}
//-----------------------------------------------------------------------------
// GLFW 윈도우 생성	
GLFWwindow* ExampleBase::CreateWindow(int width, int height, const std::string title) {
  mWindowParam.width = width;
  mWindowParam.height = height;

  if (!glfwInit()) {
    std::cerr << "glfwInit failed" << std::endl;
    return nullptr;
  }

#ifdef __APPLE__
  // macOS는 보통 OpenGL 4.1까지만 지원 + forward compatible 필요
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  glfwMakeContextCurrent(window);

  // (선택) vsync
  glfwSwapInterval(1);

  // 이벤트 콜백 등록
  glfwSetFramebufferSizeCallback(window, OnGlfwSetFramebufferSizeCallback);
  glfwSetCursorPosCallback(window, OnGlfwSetCursorPosCallback);
  glfwSetMouseButtonCallback(window, OnGlfwSetMouseButtonCallback);
  glfwSetScrollCallback(window, OnGlfwSetScrollCallback);
  glfwSetKeyCallback(window, OnGlfwSetKeyCallback);

  glfwSetWindowUserPointer(window, this);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return nullptr;
  }

  return window;
}

//-----------------------------------------------------------------------------
void ExampleBase::Run() {
	// 윈도우를 생성한다.
	mGlfwWindow = CreateWindow(800, 800, mTitle);
	if (nullptr == mGlfwWindow)
	{
		// 문제가 발생해서 종료하게 되는 경우.
		glfwTerminate();
		return;
	}
	Initialize();


	// 메인 루프
	while(!glfwWindowShouldClose(mGlfwWindow)) {
		// 화면을 특정색상으로 채움.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 컬러버퍼와 뎁스	버	퍼를 지운다.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		mDeltaTime = static_cast<float>(glfwGetTime()) - mPrevTime;

		Render();

		mPrevTime = static_cast<float>(glfwGetTime());

		// 버퍼 스왑
		glfwSwapBuffers(mGlfwWindow);
		// 이벤트 처리
		glfwPollEvents();

		// CPU 쉬는 시간 주기(점유율 조절)		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}

	// 자원 해제
	CleanUp();
	// GLFW 종료
	glfwTerminate();
}

//-----------------------------------------------------------------------------
void ExampleBase::Initialize()
{

}
//-----------------------------------------------------------------------------
void ExampleBase::Render()
{

}
//-----------------------------------------------------------------------------
void ExampleBase::CleanUp()
{

}
//-----------------------------------------------------------------------------
void ExampleBase::SetCursorVisible(bool visible) {
	if (visible) {
		glfwSetInputMode(mGlfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(mGlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
//-----------------------------------------------------------------------------
bool ExampleBase::GetKeyState(int key) {
	return glfwGetKey(mGlfwWindow, key) == GLFW_PRESS;
}

glm::vec2 ExampleBase::GetMouseWheelOffset() {
	return mMouseWheelOffset;
}
//-----------------------------------------------------------------------------
void ExampleBase::ResetMouseWheelOffset() {
	mMouseWheelOffset = glm::vec2(0.0f, 0.0f);
}

glm::vec2 ExampleBase::GetCurrentMousePosition() {
	return mMousePosition;
}
//-----------------------------------------------------------------------------
void ExampleBase::OnGlfwSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	ExampleBase* exampleWindow = static_cast<ExampleBase*>(glfwGetWindowUserPointer(window));
	exampleWindow->OnKeyInput(key, scancode, action, mods);
}
//-----------------------------------------------------------------------------
void ExampleBase::OnGlfwSetFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	ExampleBase* exampleWindow = static_cast<ExampleBase*>(glfwGetWindowUserPointer(window));
	exampleWindow->OnFramebufferSizeChanged(width, height);
}

//-----------------------------------------------------------------------------
void ExampleBase::OnGlfwSetCursorPosCallback(GLFWwindow* window, double xpos, double ypos){
	ExampleBase* exampleWindow = static_cast<ExampleBase*>(glfwGetWindowUserPointer(window));
	exampleWindow->OnMousePositionChanged(xpos, ypos);
}
//-----------------------------------------------------------------------------
void ExampleBase::OnGlfwSetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	ExampleBase* exampleWindow = static_cast<ExampleBase*>(glfwGetWindowUserPointer(window));
	exampleWindow->OnMouseButton(button, action, mods);
}
//-----------------------------------------------------------------------------
void ExampleBase::OnGlfwSetScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	ExampleBase* exampleWindow = static_cast<ExampleBase*>(glfwGetWindowUserPointer(window));
	exampleWindow->OnMouseWheelChanged(xoffset, yoffset);
}
//-----------------------------------------------------------------------------
void ExampleBase::OnFramebufferSizeChanged(int width, int height) {
	glViewport(0, 0, width, height);
	mWindowParam.width = width;
	mWindowParam.height = height;	
	//std::cout << "[OnFramebufferSizeChanged] " << width << ", " << height << std::endl;
}

void ExampleBase::OnMousePositionChanged(double xpos, double ypos) {
	mMousePosition.x = static_cast<float>(xpos);
	mMousePosition.y = static_cast<float>(ypos);
}

void ExampleBase::OnMouseButton(int button, int action, int mods) {

}

void ExampleBase::OnMouseWheelChanged(double xoffset, double yoffset) {
	mMouseWheelOffset.x += static_cast<float>(xoffset);
	mMouseWheelOffset.y += static_cast<float>(yoffset);
}

void ExampleBase::OnKeyInput(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		mKeyState[key] = true;
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(mGlfwWindow, true);
				break;
		}
	}

	else if (action == GLFW_RELEASE) {
		mKeyState[key] = false;
	}
}

