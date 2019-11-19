#version 330 core
out vec4 FragColor;

in vec2 v_texCoords;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedoSpec;

struct Light{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
    float Radius;
};
const int NR_LIGHTS = 32;
uniform Light u_lights[NR_LIGHTS];
uniform vec3 u_viewPos;

void main(){
    vec3 FragPos = texture(u_gPosition, v_texCoords).rgb;
    vec3 Normal = texture(u_gNormal, v_texCoords).rgb;
    vec4 albedo = texture(u_gAlbedoSpec, v_texCoords);
    vec3 Diffuse = albedo.rgb;
    float Specular = albedo.a;

    vec3 lighting = Diffuse * 0.1;
    vec3 viewDir = normalize(u_viewPos - FragPos);
    for (int i = 0; i < NR_LIGHTS; ++i){
        float distance = length(u_lights[i].Position - FragPos);
        if (distance < u_lights[i].Radius){
            // diffuse
            vec3 lightDir = normalize(u_lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * u_lights[i].Color;
            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = u_lights[i].Color * spec * Specular;
            // attenuation
            float attenuation = 1.0 / ( 1.0 + u_lights[i].Linear * distance + u_lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;

            lighting += diffuse + specular;
        }    	
    }
    FragColor = vec4(lighting, 1.0f);
}