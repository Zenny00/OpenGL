#shader vertex
#version 330 core

layout(location = 0) in vec4 position; // Specify the location of the vertex buffer

void main() {
    gl_Position = position; // vec4 is automatically cast to vec2 here
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; 

uniform vec4 u_Color;

void main() {
    color = u_Color;
};