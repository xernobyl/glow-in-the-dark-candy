#version 330

noperspective in vec3 zplane;

uniform vec2 zc;
uniform vec3 col0, dir0, col1, dir1, amb;
uniform sampler2DRect N, A;

out vec3 O0, O1;

const float h = 4.88e-04;
const float o[5] = float[](0.0, 1.0 * 98.0/68.0, 1.0 * 141.0/41.0, 1.0 * 130.0/24.0, 1.0 * 66.0/9.0);
const float w[5] = float[](51.0/335.0, 68.0/335.0, 41.0/335.0, 24.0/335.0, 9.0/335.0);

vec3 Position(float z)
{
	return zplane / (z * zc.x + zc.y);
}

vec3 Normal(vec2 p, out float m2)
{
	vec4 t = texture(N, gl_FragCoord.xy);
	m2 = t.w * 255.0/512.0 + 1.0/512.0;
	return normalize(t.xyz * 2.0 - 1.0);
}

float Amb(float t, float zh)
{
	float tw = w[0];
	float a = t * w[0];

	for(int i=1; i<5; i++)
	{
		vec4 t0 = texture(A, gl_FragCoord.xy + vec2(o[i], -o[i]));
		float z0 = (t0.g + t0.b / 256.0) - (t0.a - 0.5) * 0.01;

		vec4 t1 = texture(A, gl_FragCoord.xy - vec2(o[i], -o[i]));
		float z1 = (t1.g + t1.b / 256.0) - (t1.a - 0.5) * 0.01;

		float w0 = w[i] / (h + abs(zh - z0));
		float w1 = w[i] / (h + abs(zh - z1));

		tw += w0 + w1;
		a += t0.r * w0 + t1.r * w1;
	}

	return clamp(a / tw, 0.0, 1.0);
}

void main()
{	
	vec4 t = texture(A, gl_FragCoord.xy);
	float zh = t.g + t.b / 256.0;
	vec3 pos = Position(zh);
	vec3 view = normalize(pos);

	float m2;
	vec3 nor = Normal(gl_FragCoord.xy, m2);

	float d0 = max(dot(nor, dir0), 0.0);
	float d1 = max(dot(nor, dir1), 0.0);

	O0 = Amb(t.r, zh) * (col0 * d0 + col1 * d1 + amb);

	float s0, s1;

	float n0 = dot(nor, normalize(dir0 - view));
	if(n0 >= 0.0 && d0 >= 0.0)
	{
		float n02 = n0 * n0;
		s0 = exp(1.0 / m2 - 1.0 / m2 / n02) / (m2 * n02 * n02);
	}
	else
		s0 = 0.0;

	float n1 = dot(nor, normalize(dir1 - view));
	if(n1 >= 0.0 && d1 >= 0.0)
	{
		float n12 = n1 * n1;
		s1 = exp(1.0 / m2 - 1.0 / m2 / n12) / (m2 * n12 * n12);
	}
	else
		s1 = 0.0;

	O1 = col0 * s0 + col1 * s1;
}