cmake_minimum_required(VERSION 3.12)

set(PROJECTNAME "GameOfLife")

project(${PROJECTNAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

message("
Generator: ${CMAKE_GENERATOR}
Build: ${CMAKE_BUILD_TYPE} 
CompilerId: ${CMAKE_CXX_COMPILER_ID}
")

# VENDORED
set(GLM_INCLUDE_DIR vendor/glm)
set(IMGUI_INCLUDE_DIR vendor/imgui)
set(GLFW_INCLUDE_DIR vendor/GLFW/include)
set(GLAD_INCLUDE_DIR vendor/glad/include)
include_directories(
  ${GLM_INCLUDE_DIR}
  ${IMGUI_INCLUDE_DIR}
  ${GLAD_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
)

# add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
add_subdirectory(vendor)

add_definitions(-pthread)
add_definitions(-DGLFW_INCLUDE_NONE)

file(GLOB_RECURSE SRCS
	GameOfLife/*.cpp
)

file(GLOB_RECURSE HEADERS
	GameOfLife/*.h
)

add_executable(${PROJECTNAME} ${HEADERS} ${SRCS})

find_package(OpenGL REQUIRED)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_options("${PROJECTNAME}" PRIVATE "-Wall" "-Wextra")
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "gcc")
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_options("${PROJECTNAME}" PRIVATE "-Wall" "-Wextra")
  endif()
endif()

target_link_libraries(${PROJECTNAME}
  glfw
  glad
  imgui
  pthread
  OpenGL::GL
  ${CMAKE_DL_LIBS}
)
