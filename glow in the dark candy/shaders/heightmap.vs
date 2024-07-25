#version 330

in vec4 A0;

uniform mat4 m, hm;
uniform texture2D H;

void main()
{
	vec4 gridpos = hm * A0;
	gridpos /= gridpos.w;

	float h = texture(H, xxx);
	gridpos.y += h;
	
	gl_Position = m * gridpos;
}