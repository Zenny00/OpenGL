cmake_minimum_required(VERSION 3.0.0)
project(opengl_simulation VERSION 0.1)

add_executable(opengl_simulation 
/home/zenny/Desktop/OpenGL/src/main.cpp 
/home/zenny/Desktop/OpenGL/src/glad.c
)

target_link_libraries(opengl_simulation glfw)