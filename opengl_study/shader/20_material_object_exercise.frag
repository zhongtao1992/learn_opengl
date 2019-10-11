#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
    sampler2D emission;
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
in vec2 oTexCoords;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform Material u_material;
uniform Light u_light;

void main(){
    // ambient
    vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, oTexCoords));

    // diffuse
    vec3 norm = normalize(oNormal);
    vec3 lightDir = normalize(u_lightPos - oFragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = u_light.diffuse * (diff * vec3(texture(u_material.diffuse, oTexCoords)));

    // specular
    vec3 viewDir = normalize(u_viewPos - oFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, oTexCoords));

    // emission
    vec3 emission = texture(u_material.emission, oTexCoords).rgb;

    vec3 result = (ambient + diffuse + specular + emission);
    FragColor = vec4(result, 1.0f);
}