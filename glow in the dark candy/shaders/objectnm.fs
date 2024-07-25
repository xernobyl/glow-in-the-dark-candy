#version 330

out vec4 O0;

in mat3 tbn;
in vec2 tc;

uniform float s;
uniform sampler2D N;
uniform mat4 nm;

void main()
{
	vec3 nor = texture(N, tc).xyz - 0.5;
	nor = tbn * nor;

	vec3 t = abs(nor);
	float m = max(t.x, max(t.y, t.z));
	O0 = vec4(nor/m * 0.5 + 0.5, s);
}