#version 330

uniform sampler2DRect S;

//float o[3] = float[](0.0, 1.3846153846, 3.2307692308);
//float w[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

float o[5] = float[](0.0, 98.0/68.0, 141.0/41.0, 130.0/24.0, 66.0/9.0);
float w[5] = float[](51.0/335.0, 68.0/335.0, 41.0/335.0, 24.0/335.0, 9.0/335.0);

out vec3 O0;

void main()
{
	O0 = texture(S, gl_FragCoord.xy).rgb * w[0];

	for(int i=1; i<5; i++)
	{
		O0 += texture(S, gl_FragCoord.xy + vec2(o[i], -o[i])).rgb * w[i];
		O0 += texture(S, gl_FragCoord.xy - vec2(o[i], -o[i])).rgb * w[i];
	}
}