#version 330

invariant out vec4 gl_Position;

in vec4 A0;
in vec2 A1;

uniform mat4 m;

out vec2 t;

void main()
{
	gl_Position = m * A0;
	t = A1;
}