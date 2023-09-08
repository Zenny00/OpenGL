#shader vertex
#version 330 core

layout(location = 0) in vec4 position; // Specify the location of the vertex buffer
layout(location = 1) in vec2 texCoord; // Specify the location of the vertex buffer

out vec2 v_TexCoord;

void main() {
    gl_Position = position; // vec4 is automatically cast to vec2 here
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; 

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
};