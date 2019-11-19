#version 330 core
out vec4 FragColor;

in vec2 v_texCoords;

uniform sampler2D u_texture;
uniform bool u_horizontal;
uniform float u_weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main(){
    vec2 tex_offset = 1.0 / textureSize(u_texture, 0);
    vec3 result = texture(u_texture, v_texCoords).rgb * u_weight[0];

    if (u_horizontal){
        for (int i = 1; i < 5; ++i){
            result += texture(u_texture, v_texCoords + vec2(tex_offset.x * i, 0.0)).rgb * u_weight[i];
            result += texture(u_texture, v_texCoords - vec2(tex_offset.x * i, 0.0)).rgb * u_weight[i];
        }
    }
    else{
        for (int i = 1; i < 5; ++i){
            result += texture(u_texture, v_texCoords + vec2(0.0, tex_offset.y * i)).rgb * u_weight[i];
            result += texture(u_texture, v_texCoords - vec2(0.0, tex_offset.y * i)).rgb * u_weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}