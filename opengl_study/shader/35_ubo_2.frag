#version 330 core
out vec4 FragColor;

in vec2 inTexCoords;
uniform sampler2D u_texture;

void main(){
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}