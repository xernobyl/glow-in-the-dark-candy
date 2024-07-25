#version 330

uniform sampler2DRect S;
out vec3 O0;

void main()
{
	ivec2 c = ivec2(gl_FragCoord.xy * 2.0);

	vec3 s0 = texelFetch(S, c - ivec2(0, 0)).rgb;
	vec3 s1 = texelFetch(S, c - ivec2(1, 0)).rgb;
	vec3 s2 = texelFetch(S, c - ivec2(0, 1)).rgb;
	vec3 s3 = texelFetch(S, c - ivec2(1, 1)).rgb;

	O0 = max(s0, max(s1, max(s2, s3)));
	
	O0 /= (O0 + 1.0);
	O0 -= .5;

	O0 = 2.0 * max(O0, 0.0);

	//O0 = log2(max(vec3(1.0), O0));
	//O0 = pow(O0, vec3(4.0));
}