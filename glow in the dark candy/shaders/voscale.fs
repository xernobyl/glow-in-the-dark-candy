#version 330

out vec3 O0;

uniform sampler2DRect A, Z;
uniform float at;
uniform vec2 zc;

float LinearDepth(ivec2 p)
{
	float z = texelFetch(Z, p).r;
	return 1.0 / (z * zc.x + zc.y);
}

void main()
{
	//O0 = texture(A, gl_FragCoord.xy * 0.5).rrr;
	//return;
	
	vec4 blw2[4];
	blw2[0] = vec4(.1875, .5625, .0625, .1875);
	blw2[1] = vec4(.5625, .1875, .1875, .0625);
	// mirror
	blw2[2] = vec4(.0625, .1875, .1875, .5625);
	blw2[3] = vec4(.1875, .0625, .5625, .1875);

	ivec2 fragcoord = ivec2(gl_FragCoord.xy);

	float depth = LinearDepth(fragcoord);
	
	ivec2 b = fragcoord & 1;
	int texel2 = b.x + 2*b.y;
	
	ivec2 p = fragcoord/2 + b;

	vec2 aa[4];
	aa[0] = texelFetchOffset(A, p, ivec2(-1, 0)).rg;
	aa[1] = texelFetchOffset(A, p, ivec2(0, 0)).rg;
	aa[2] = texelFetchOffset(A, p, ivec2(-1, -1)).rg;
	aa[3] = texelFetchOffset(A, p, ivec2(0, -1)).rg;
	
	float atw, itw, ambient;

	for(int i = 0; i < 4; ++i)
	{
		float aw = blw2[texel2][i] / (at + abs(depth - aa[i].g));
		atw += aw;
		ambient += aa[i].r * aw;
	}

	O0 = vec3(ambient / atw);
}