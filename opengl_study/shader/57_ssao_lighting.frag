#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec2 v_texCoords;
    vec3 v_fragPos;
    vec3 v_normal;
} fs_in;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedo;
uniform sampler2D u_ssao;

struct Light{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
};
const int NR_LIGHTS = 32;
uniform Light u_light;
uniform vec3 u_viewPos;

void main(){
    vec3 FragPos = texture(u_gPosition, fs_in.v_texCoords).rgb;
    vec3 Normal = texture(u_gNormal, fs_in.v_texCoords).rgb;
    vec3 Diffuse = texture(u_gAlbedo, fs_in.v_texCoords).rgb;
    float ambientOcclusion = texture(u_ssao, fs_in.v_texCoords).r;

    // 
    vec3 ambient = vec3(0.3 * Diffuse * ambientOcclusion);
    vec3 lighting = ambient;
    vec3 viewDir = normalize(-FragPos);
    // diffuse 
    vec3 lightDir = normalize(u_light.Position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * u_light.Color;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
    vec3 specular = u_light.Color * spec;
    // attenuation
    float distance = length(u_light.Position - FragPos);
    float attenuation = 1.0 / (1.0 + u_light.Linear * distance + u_light.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    FragColor = vec4(lighting, 1.0f);
}