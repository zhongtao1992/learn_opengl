#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D u_diffuseTex;
uniform sampler2D u_shadowMap;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
	// 
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// 
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0){
		return 0.0;
	}
	//float closestDepth = texture(u_shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	// fix shadow acne
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	//return currentDepth - bias > closestDepth ? 1.0 : 0.0;

	// fix shadow jag pcf(percentage closer filtering)
	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
	for (int x = -1; x <= 1; ++x){
		for (int y = -1; y <= 1; ++y){
			float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0f;
	return shadow;
}

void main(){
	vec3 color = texture(u_diffuseTex, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);

	vec3 lightColor = vec3(0.3);
	// ambient
	vec3 ambient = 0.3 * lightColor;
	// diffuse 
	vec3 lightDir = normalize(u_lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(u_viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float shadow = shadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0f);
}