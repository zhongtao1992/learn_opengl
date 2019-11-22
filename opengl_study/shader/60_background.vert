#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 v_worldPos;

uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
	v_worldPos = aPos;

	mat4 rotView = mat4(mat3(u_view));
	vec4 clipPos = u_projection * rotView * vec4(aPos, 1.0f);

    gl_Position = clipPos.xyww;
}