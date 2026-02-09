cmake_minimum_required(VERSION 3.16)
project(MyOpenGLStudy LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL 찾기
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

# 소스 파일들
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})



# ✅ MSVC에서만, 그리고 내 실행 타겟에만 UTF-8 옵션 적용
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
  # 필요하면 아래 2줄까지(대부분은 /utf-8만으로 충분)
  # target_compile_options(${PROJECT_NAME} PRIVATE /source-charset:utf-8)
  # target_compile_options(${PROJECT_NAME} PRIVATE /execution-charset:utf-8)

  
  set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
  )
endif()

# GLFW 헤더가 OpenGL 헤더를 포함하지 않도록 설정
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# 인클루드 디렉토리 설정
target_include_directories(${PROJECT_NAME} PRIVATE
  src
  externals
  externals/glad/include
  externals/glm
  
)

# 링크 설정
target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  glad
  OpenGL::GL
  glm
)


# 빌드 폴더(CMAKE_BINARY_DIR)로 resources 폴더를 통째로 복사
file(COPY ${CMAKE_SOURCE_DIR}/resources 
     DESTINATION ${CMAKE_BINARY_DIR})
