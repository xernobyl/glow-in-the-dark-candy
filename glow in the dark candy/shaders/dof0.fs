#version 330

uniform sampler2DRect S;
uniform vec2 step0, step1;

out vec4 O0, O1;

const int NSAMPLES = 6;
const float STEPSIZE = 1.0 / (NSAMPLES - 1);

void main()
{
	float coc = texture(S, gl_FragCoord.xy).a;

	for(int i=1; i<NSAMPLES; ++i)
		O1 += texture(S, gl_FragCoord.xy + step0 * coc * float(i) * STEPSIZE);

	O0 = O1;
	O1 /= NSAMPLES - 1;

	for(int i=0; i<NSAMPLES; ++i)
		O0 += texture(S, gl_FragCoord.xy + step1 * coc * float(i) * STEPSIZE);

	O0 = O0 / (NSAMPLES*2 - 1);
}