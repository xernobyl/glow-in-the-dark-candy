#version 330

//#define USE_NORMALS

uniform sampler2DRect Z, N;
uniform vec2 zc;

float LinearDepth(vec2 p)
{
	float z = texture(Z, p).r;
	return 1.0 / (z * zc.x + zc.y);
}

#ifdef USE_NORMALS
vec3 Normal(vec2 p)
{
	return normalize(texture(N, p).xyz * 2.0 - 1.0);
}

vec3 n;
#endif

out vec2 O0;

const int nsamples = 12;

uniform vec3 sample[nsamples];
uniform float wc;
uniform sampler2D R;


float Pz, weight;

float O(vec2 p, vec2 s, float r)
{
	float dz = LinearDepth(p) - Pz;

	#ifdef USE_NORMALS
		float nd = dot(s, n.xy);
		weight += r + min(r, nd);
		return max(min(min(r, nd), dz) + r, 0.0);
	#else
		weight += r;
		return max(min(r, dz) + r, 0.0);
	#endif
}


void main()
{
	Pz = LinearDepth(gl_FragCoord.xy * 2.0);

	#ifdef USE_NORMALS
		n = Normal(gl_FragCoord.xy * 2.0);
		n.xy /= n.z;
	#endif

	weight = 0.0;
	float o = 0.0;
	vec2 rn = normalize(texture(R, gl_FragCoord.xy / 32.0).xy - 0.5);

	for(int i=0; i<nsamples; ++i)
	{
		vec2 s = reflect(sample[i].xy, rn);
		o += O(gl_FragCoord.xy*2.0 + wc * s / Pz, s, sample[i].z);
	}

	O0 = vec2(clamp(o / weight, 0.0, 1.0), Pz);
}