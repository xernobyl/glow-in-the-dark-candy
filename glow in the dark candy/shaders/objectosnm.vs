#version 330

invariant out vec4 gl_Position;

in vec4 A0;
in vec2 A1;

out vec2 tc;

uniform mat4 m;

void main()
{
	tc = A1;
	gl_Position = m * A0;
}