#version 330 core
out vec4 FragColor;

in vec2 inTexCoords;
uniform sampler2D u_texture;

void main(){
	vec4 textureColor = texture(u_texture, inTexCoords);
	if (textureColor.a < 0.1){
		discard;
	}
    FragColor = textureColor;
}