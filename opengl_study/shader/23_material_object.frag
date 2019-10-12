#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;	
};
struct Light {
	vec3 position;
    vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
};

out vec4 FragColor;
in vec3 oNormal;
in vec3 oFragPos;
in vec2 oTexCoords;

uniform vec3 u_viewPos;
uniform Material u_material;
uniform Light u_light;

void main(){
    vec3 lightDir = normalize(u_light.position - oFragPos);
    // cutoff
    float theta = dot(lightDir, normalize(-u_light.direction));
    if (theta > u_light.cutOff){
        // attenuation
        float distance = length(u_light.position - oFragPos);
        float attenuation = 1.0 / (u_light.constant + u_light.linear * distance + u_light.quadratic * (distance * distance));

        // ambient
        vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, oTexCoords));
        ambient *= attenuation;

        // diffuse
        vec3 norm = normalize(oNormal);    
        float diff = max(dot(norm, lightDir), 0.0f);
        vec3 diffuse = u_light.diffuse * (diff * vec3(texture(u_material.diffuse, oTexCoords)));
        diffuse *= attenuation;

        // specular
        vec3 viewDir = normalize(u_viewPos - oFragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_material.shininess);
        vec3 specular = u_light.specular * (spec * vec3(texture(u_material.specular, oTexCoords)));
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular);
        FragColor = vec4(result, 1.0f);
    }
    else{
        FragColor = vec4(u_light.ambient * vec3(texture(u_material.diffuse, oTexCoords)), 1.0f);
    }
}