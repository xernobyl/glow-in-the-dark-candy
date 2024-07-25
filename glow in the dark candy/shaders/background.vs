#version 330

in vec4 A0;

void main()
{
	gl_Position = vec4(A0.xy, 1.0, 1.0);
}