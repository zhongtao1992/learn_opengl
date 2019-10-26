#version 330 core
out vec4 FragColor;

in vec2 inTexCoords;
uniform sampler2D u_texture;

void main(){
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}