#version 330 core
out vec4 FragColor;

in vec3 v_worldPos;

uniform samplerCube u_environmentMap;

void main(){
	vec3 envColor = texture(u_environmentMap, v_worldPos).rgb;

	// hdr
	envColor = envColor / (envColor + vec3(1.0f));
	// gamma
	envColor = pow(envColor, vec3(1.0 / 2.2));

	FragColor = vec4(envColor, 1.0f);
}