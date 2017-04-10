#version 330 core

uniform mat4 model_view_proj;
uniform mat4 normal_trans;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT{
	vec3 normal;
}vs;

void main(void) {
	gl_Position = model_view_proj * vec4(position,1);
	//normal = normalize(ciNormalMatrix * ciNormal);
	vs.normal = vec3(normalize(normal_trans * vec4(normal, 0)));
}