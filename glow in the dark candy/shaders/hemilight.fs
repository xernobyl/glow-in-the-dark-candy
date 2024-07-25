#version 330

out vec3 O0, O1;

noperspective in vec3 zplane;

uniform vec3 col0, dir0, col1, dir1, amb;
uniform vec2 zc;
uniform sampler2DRect N, Z;

vec3 Position(vec2 p)
{
	float z = texture(Z, p).r;
	return zplane / (z * zc.x + zc.y);
}

vec3 Normal(vec2 p, out float m2)
{
	vec4 t = texture(N, gl_FragCoord.xy);
	m2 = t.w * 255.0/512.0 + 1.0/512.0;
	return normalize(t.xyz * 2.0 - 1.0);
}

void main()
{	
	float m2;

	vec3 pos = Position(gl_FragCoord.xy);
	vec3 view = normalize(pos);
	vec3 nor = Normal(gl_FragCoord.xy, m2);

	float d0 = max(dot(nor, dir0), 0.0);
	float d1 = max(dot(nor, dir1), 0.0);

	O0 = col0 * d0 + col1 * d1 + amb;
	
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