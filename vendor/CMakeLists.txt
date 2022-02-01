set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(GLFW)

file(GLOB GLAD_SOURCES glad/src/glad.c)
file(GLOB GLAD_HEADERS glad/include/glad/glad.h glad/include/KHR/khrplatform.h)
set(GLAD_INCLUDES glad/include)
include_directories(${GLAD_INCLUDES})
add_library(glad STATIC
  ${GLAD_HEADERS}
  ${GLAD_SOURCES}
)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(imgui)

add_subdirectory(glm)
