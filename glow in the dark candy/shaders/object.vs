#version 330

invariant out vec4 gl_Position;

in vec4 A0;
in vec3 A2;

out vec3 n;
uniform mat4 m, nm;

void main()
{
	gl_Position = m * A0;
	n = (nm * vec4(A2, 0.0)).xyz;
}