#version 330

uniform vec2 s = vec2(1.0);
uniform vec2 b = vec2(0.0);
uniform sampler2DRect S;

out vec4 O0;

void main()
{
	O0 = texture(S, gl_FragCoord.xy * s + b);
}