#version 330

uniform sampler2DRect S0, S1;
uniform vec2 step1, step2;

out vec3 O0;

void main()
{
	vec4 t = texture(S0, gl_FragCoord.xy);
	float coc = t.a;

	O0 = t.rgb;

	for(int i=1; i<32; ++i)
	{
		t = texture(S0, gl_FragCoord.xy + step2 * coc * float(i) / 31.0);
		O0 += t.rgb;
	}

	O0 *= 2.0;

	for(int i=1; i<32; ++i)
	{
		t = texture(S1, gl_FragCoord.xy + step1 * coc * float(i) / 31.0);
		O0 += t.rgb;
	}

	O0 /= 93.0 * coc;
}