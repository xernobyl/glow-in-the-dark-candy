#version 330

invariant out vec4 gl_Position;

in vec4 A0;
in vec2 A1;
in vec3 A2, A3, A4;

out mat3 tbn;
out vec2 tc;

uniform mat4 m, nm;

void main()
{
	tbn = mat3(nm) * mat3(normalize(A4), normalize(A3), normalize(A2));
	tc = A1;
	gl_Position = m * A0;
}