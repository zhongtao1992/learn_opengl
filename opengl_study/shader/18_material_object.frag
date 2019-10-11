#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;	
};
struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;
in vec3 oNormal;
in vec3 oFragPos;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform Material u_material;
uniform Light u_light;

void main(){
    vec3 ambient = u_light.ambient * u_material.ambient;

    vec3 norm = normalize(oNormal);
    vec3 lightDir = normalize(u_lightPos - oFragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);

    vec3 viewDir = normalize(u_viewPos - oFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = u_light.specular * (spec * u_material.specular);

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0f);
}