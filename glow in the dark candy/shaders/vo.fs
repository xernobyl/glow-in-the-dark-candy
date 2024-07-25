#version 330

const float NOISE_RES = 32.0;
const int MAX_SAMPLES = 24;

uniform sampler2DRect Z;
uniform sampler2D R;
uniform vec2 zc;
uniform float tw;
uniform vec3 sample[MAX_SAMPLES];
uniform float r;

float Pz;

out vec4 O0;

float LinearDepth(vec2 p)
{
	float z = texture(Z, p).r;
	return 1.0 / (z * zc.x + zc.y);
}

float O(vec2 p, float r)
{
	float dz = LinearDepth(p) - Pz;
	return max(min(r, dz), -r);
}

void main()
{
	float zzz = texture(Z, gl_FragCoord.xy * 2.0).r;
	Pz = LinearDepth(gl_FragCoord.xy * 2.0);

	float o = 0.0;
	vec2 rn = normalize(texture(R, gl_FragCoord.xy / NOISE_RES).xy - 0.5);

	int nsamples = MAX_SAMPLES;
	
	for(int i=0; i<nsamples; ++i)
	{
		vec2 s = reflect(sample[i].xy, rn);	//vec2 s = sample[i].xy;
		o += O(gl_FragCoord.xy*2.0 + r * s, sample[i].z);
	}

	float u = floor(zzz * 256.0);
	float res_u = zzz * 256.0 - u;
	float v = floor(res_u * 256.0);
	float res_v = res_u * 256.0 - v;
	float w = floor(res_v * 256.0);

	O0 = vec4(o * tw + 1.0, vec3(u, v, w) / 256.0);
}