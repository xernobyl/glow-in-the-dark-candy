#version 330

uniform sampler2DRect S, S0, S1;
uniform vec2 step1, step2;

out vec3 O0;

const int NSAMPLES = 6;
const float STEPSIZE = 1.0 / (NSAMPLES - 1);

void main()
{
	float coc = texture(S, gl_FragCoord.xy).a;
	vec4 accum = vec4(0.0);

	for(int i=0; i<NSAMPLES; ++i)
		accum += texture(S0, gl_FragCoord.xy + step2 * coc * float(i) * STEPSIZE);

	accum *= 2.0;

	for(int i=1; i<NSAMPLES; ++i)
		accum += texture(S1, gl_FragCoord.xy + step1 * coc * float(i) * STEPSIZE);

	accum /= 3 * NSAMPLES - 1;
	O0 = accum.rgb / accum.a;
}