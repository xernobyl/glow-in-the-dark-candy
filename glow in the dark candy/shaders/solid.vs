#version 330

invariant out vec4 gl_Position;

in vec4 A0;

uniform mat4 m;

void main()
{
	gl_Position = m * A0;
}