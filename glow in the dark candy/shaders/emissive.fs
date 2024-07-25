#version 330

out vec4 O0;

uniform vec2 c;
uniform vec3 ec;

void main()
{
	float coc = abs(gl_FragCoord.w * c.x + c.y);
	O0 = vec4(ec, coc);
}