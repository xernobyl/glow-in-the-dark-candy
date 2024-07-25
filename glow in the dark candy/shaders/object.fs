#version 330

out vec4 O0;
in vec3 n;

uniform float s;

void main()
{
	vec3 t = abs(n);
	float m = max(t.x, max(t.y, t.z));
	O0 = vec4(n/m * 0.5 + 0.5, s);
}