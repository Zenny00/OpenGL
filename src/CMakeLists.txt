cmake_minimum_required(VERSION 3.0.0)
project(opengl_simulation VERSION 0.1)

add_executable(opengl_simulation 
    main.cpp 
    glad.c
    IndexBuffer.cpp
    VertexBuffer.cpp
    ErrorChecker.cpp
    VertexArray.cpp
    VertexBuffer.cpp
    Shader.cpp
    Renderer.cpp
    ./vendor/stb_image/stb_image.cpp
    Texture.cpp
)

target_link_libraries(opengl_simulation glfw)