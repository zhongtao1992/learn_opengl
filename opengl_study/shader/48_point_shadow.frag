#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D u_diffuseTex;
uniform samplerCube u_shadowMap;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform float u_far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 

float shadowCalculation(vec3 fragPos, vec3 normal, vec3 lightDir){
	vec3 fragToLight = fragPos - u_lightPos;
	if (fragToLight.z > 1.0){
		return 0.0;
	}
	//float closestDepth = texture(u_shadowMap, projCoords.xy).r;
	float currentDepth = length(fragToLight);
	// fix shadow acne
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	//return currentDepth - bias > closestDepth ? 1.0 : 0.0;

	// fix shadow jag pcf(percentage closer filtering)
	float shadow = 0.0f;
	int samples = 20;
	float viewDis = length(u_viewPos - fragPos);
	float diskRadius = (1.0 + (viewDis / u_far_plane)) / 25.0;
	for (int i = 0; i < samples; ++i){
		float closestDepth = texture(u_shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= u_far_plane;
		if (currentDepth - bias > closestDepth){
			shadow += 1.0;
		}
	}
	shadow /= float(samples);
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

	float shadow = shadowCalculation(fs_in.FragPos, normal, lightDir);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0f);
}