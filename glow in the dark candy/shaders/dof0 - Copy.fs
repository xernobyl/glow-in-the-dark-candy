#version 330

uniform sampler2DRect S;
uniform vec2 step0, step1;

out vec4 O0;	// | + /,	0 + 1
out vec3 O1;	// |,		0

const int NSAMPLES = 16;

void main()
{
	vec3 accum;

	vec4 t = texture(S, gl_FragCoord.xy);
	float coc = t.a;

	for(int i=1; i<32; ++i)
	{
		t = texture(S, gl_FragCoord.xy + step0 * coc * float(i) / 31.0);
		accum += t.rgb;
	}

	O1 = accum / 31.0;
	
	for(int i=0; i<32; ++i)
	{
		t = texture(S, gl_FragCoord.xy + step1 * coc * float(i) / 31.0);
		accum += t.rgb;
	}

	O0 = vec4(accum / 63.0, coc);
}