set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(GLFW)

# For glad2 -- which did not work as expected...
#file(GLOB GLAD_SOURCES glad/src/gl.c)
#file(GLOB GLAD_HEADERS glad/include/glad/gl.h glad/include/KHR/khrplatform.h)
file(GLOB GLAD_SOURCES glad/src/glad.c)
file(GLOB GLAD_HEADERS glad/include/glad/glad.h glad/include/KHR/khrplatform.h)
set(GLAD_INCLUDES glad/include)
include_directories(${GLAD_INCLUDES})
add_library(glad STATIC
  ${GLAD_HEADERS}
  ${GLAD_SOURCES}
)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)

# Copy needed imgui_impl_xxx.* files to imgui/ root dir for convenience:
#message("Copy imgui backend/ impl files")
# file(COPY imgui/backends/imgui_impl_glfw.h DESTINATION imgui/)
# file(COPY imgui/backends/imgui_impl_glfw.cpp DESTINATION imgui/)
# file(COPY imgui/backends/imgui_impl_opengl3.cpp DESTINATION imgui/)
# file(COPY imgui/backends/imgui_impl_opengl3.h DESTINATION imgui/)
# file(COPY imgui/backends/imgui_impl_opengl3_loader.h DESTINATION imgui/)

# set(IMGUI_BACKENDS imgui/backends)
# include_directories(${IMGUI_BACKENDS})
add_subdirectory(imgui)

add_subdirectory(glm)
