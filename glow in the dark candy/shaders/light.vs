#version 330

in vec4 A0;
uniform vec2 hfov;
noperspective out vec3 zplane;

void main()
{
	gl_Position = A0;
	zplane = vec3(hfov * A0.xy, -1.0);
}