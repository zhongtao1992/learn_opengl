#version 330 core
out vec4 FragColor;

in vec2 inTexCoords;
uniform sampler2D u_texture;

void main(){
    FragColor = texture(u_texture, inTexCoords);
}