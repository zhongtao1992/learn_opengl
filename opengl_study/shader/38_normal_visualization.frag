#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;

    sampler2D texture_diffuse1;
	float shininess;	
};

uniform Material u_material;
in vec2 v_texCoords;

void main(){
    FragColor = texture(u_material.texture_diffuse1, v_texCoords);
}