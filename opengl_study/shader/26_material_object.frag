#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;

    sampler2D texture_diffuse1;
	float shininess;	
};

out vec4 FragColor;
in vec3 oNormal;
in vec3 oFragPos;
in vec2 oTexCoords;

uniform vec3 u_viewPos;
uniform Material u_material;

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight u_dirLight;
vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // ambient
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, oTexCoords));
    // diffuse
    vec3 norm = normalize(normal);    
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(u_material.diffuse, oTexCoords)));
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(u_material.specular, oTexCoords)));

    return ambient + diffuse + specular;
}

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
};
#define NR_POINT_LIGHT 4
uniform PointLight u_pointLights[NR_POINT_LIGHT];
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 lightDir = normalize(light.position - fragPos);
    // ambient
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, oTexCoords));
    ambient *= attenuation;

    // diffuse
    vec3 norm = normalize(normal);    
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(u_material.diffuse, oTexCoords)));
    diffuse *= attenuation;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(u_material.specular, oTexCoords)));
    specular *= attenuation;

    return ambient + diffuse + specular;
}

struct SpotLight{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};
uniform SpotLight u_spotLight;
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // cutoff
    float theta = dot(lightDir, normalize(-light.direction));
    // epsilon
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    // attenuation
    float distance = length(light.position - oFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // ambient
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, oTexCoords));   
    ambient *= attenuation * intensity;

    // diffuse
    vec3 norm = normalize(normal);    
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(u_material.diffuse, oTexCoords)));
    diffuse *= attenuation * intensity;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(u_material.specular, oTexCoords)));
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

void main(){
    /*// properties
    vec3 norm = normalize(oNormal);
    vec3 viewDir = normalize(u_viewPos - norm);

    // direction light
    vec3 result = calculateDirLight(u_dirLight, norm, viewDir);

    // point light
    for (int i = 0; i < NR_POINT_LIGHT; ++i){
        result += calculatePointLight(u_pointLights[i], norm, oFragPos, viewDir);
    }

    // spot light
    result += calculateSpotLight(u_spotLight, norm, oFragPos, viewDir);

    FragColor = vec4(result, 1.0f);*/

    FragColor = texture(u_material.texture_diffuse1, oTexCoords);
}