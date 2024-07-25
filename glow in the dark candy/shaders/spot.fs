#version 330

out vec3 O0, O1;

noperspective in vec3 zplane;

uniform vec3 color, position;
uniform vec2 zc, sp;
uniform sampler2DRect N, Z;

vec3 Position(vec2 p)
{
	float z = texture(Z, p).r;
	return zplane / (z * zc.x + zc.y);
}

vec3 Normal(vec2 p, out float sh)
{
	vec4 t = texture(N, gl_FragCoord.xy);
	sh = t.w * 255.0/256.0 + 1.0/256.0;
	return normalize(t.xyz * 2.0 - 1.0);
}

void main()
{	
	float sh;

	vec3 pos = Position(gl_FragCoord.xy);
	vec3 view = normalize(pos);
	vec3 nor = Normal(gl_FragCoord.xy, sh);
	
	vec3 direction = position - pos;
	float distance = length(direction);
	direction /= distance;

	float d = dot(nor, direction);
	
	//float spot = dot(ld, direction) * sp.x + sp.y;
	
	if(d < 0.0)
		discard;
		
	float n = dot(nor, normalize(direction - view));

	if(n < 0.0)
		discard;

	O0 = color * d * pow(2.0, -distance);

	float s =  exp(1.0 / m2 - 1.0 / (n2 * m2)) / (m2 * n2 * n2);
	O1 = color * s;
}