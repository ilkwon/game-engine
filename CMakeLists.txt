cmake_minimum_required(VERSION 3.16)
project(MyOpenGLStudy LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# GLFW (소스 포함 빌드)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(externals/glfw)

# GLAD (소스 포함 빌드)
add_library(glad STATIC externals/glad/src/glad.c)
target_include_directories(glad PUBLIC externals/glad/include)

# GLM (헤더만)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE externals/glm)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

target_include_directories(${PROJECT_NAME} PRIVATE
  src
  externals
  externals/glad/include
  externals/glm
  
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  glad
  OpenGL::GL
  glm
)


# 빌드 폴더(CMAKE_BINARY_DIR)로 resources 폴더를 통째로 복사
file(COPY ${CMAKE_SOURCE_DIR}/resources 
     DESTINATION ${CMAKE_BINARY_DIR})

# [추가] Visual Studio 디버깅 시 작업 디렉터리를 '실행 파일이 있는 곳'으로 강제 설정
set_target_properties(${PROJECT_NAME} PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
)