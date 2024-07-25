#version 330

out vec3 O0, O1;

noperspective in vec3 zplane;

uniform vec3 color, position;
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

	vec3 direction = position - pos;
	float distance = length(direction);
	direction /= distance;

	float d = dot(nor, direction);

	if(d < 0.0)
		discard;

	float a = 1.0 / distance / distance;
	O0 = color * d * a;

	float n = dot(nor, normalize(direction - view));
	if(n > 0.0)
	{
		float n2 = n * n;
		float s = exp(1.0 / m2 - 1.0 / m2 / n2) / (m2 * n2 * n2);
		O1 = color * s * a;
	}
	else
		O1 = vec3(0.0);
}