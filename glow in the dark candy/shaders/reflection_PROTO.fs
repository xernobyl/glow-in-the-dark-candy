#version 330

in vec3 n, p;

uniform samplerCube S;

uniform float z;
uniform vec3 origin;
uniform vec4 co;

void main()
{
	vec3 normal = normalize(n);
	vec3 view = normalize(p - co.xyz);

	vec3 o = origin.xyz;

	vec3 t = o + reflect(view, normal) * texture(S, normal);

	gl_FragColor = texture(S, t + p - 2 * o);
}