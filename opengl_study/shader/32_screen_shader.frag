#version 330 core
out vec4 FragColor;

in vec2 inTexCoords;
uniform sampler2D u_screenTexture;

// impact the sharpness
const float offset = 1.0 / 300.0;
vec3 kernelSample(float kernel[9]){
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),  // top left
		vec2(0.0f, offset),		// top center
		vec2(offset, offset),	// top right
		vec2(-offset, -0.0f),	// center-left
		vec2(0.0f, 0.0f),		// center-center
		vec2(offset, 0.0f),		// center-right
		vec2(-offset, -offset), // bottom-left
		vec2(0.0f, -offset),	// bottom-center
		vec2(offset, -offset)	// bottom-rigth
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; ++i){
		sampleTex[i] = vec3(texture(u_screenTexture, inTexCoords.st + offsets[i]));	
	}
	vec3 sampleColor = vec3(0.0f);
	for(int i = 0; i < 9; ++i){
		sampleColor += sampleTex[i] * kernel[i];
	}
	return sampleColor;
}

void main(){
	// normal
	vec3 col = texture(u_screenTexture, inTexCoords).rgb;
	FragColor = vec4(col, 1.0);

	// inversion effect
	FragColor = vec4(1.0 - col, 1.0);

	// grayscale effect
	float average = col.r * 0.2126 + col.g * 0.7152 + col.b * 0.0722;
	FragColor = vec4(average, average, average, 1.0);

	// sharpen kernel 
	float sharpenKernel[9] = float[](
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	);
	FragColor = vec4(kernelSample(sharpenKernel), 1.0f);

	// blur effect
	float blurKernel[9] = float[](
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
		2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
	);
	FragColor = vec4(kernelSample(blurKernel), 1.0f);

	// edge detection
	float edgeKernel[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
	);
	FragColor = vec4(kernelSample(edgeKernel), 1.0f);
}