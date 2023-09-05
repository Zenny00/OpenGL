#shader vertex
#version 330 core

layout(location = 0) in vec4 position; // Specify the location of the vertex buffer

void main() {
    gl_Position = position; // vec4 is automatically cast to vec2 here
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; 

void main() {
    color = vec4(0.2, 0.3, 0.8, 1.0); // Color value for red
};