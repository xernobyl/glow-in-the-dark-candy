#version 330

uniform sampler2DRect S;
out vec4 O0;

void main()
{
	O0 = texture(S, gl_FragCoord.xy * 2.0);
	O0.a *= 0.5;
	O0.rgb *= O0.a;
}