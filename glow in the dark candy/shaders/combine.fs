#version 330

uniform sampler2DRect A, D, G;
out vec4 O0;

void main()
{
	vec4 t = texture(A, gl_FragCoord.xy);
	float coc = clamp(abs(t.a) - 1.0, 0.0, 1.0);

	//vec3 a = t.rgb;
	vec3 a = mix(t.rgb, texture(D, gl_FragCoord.xy * 0.5).rgb, coc);

	a /= (a + 1.0);
	
	a += texture(G, gl_FragCoord.xy * 0.25).rgb;
	
	O0 = vec4(a, sqrt(dot(a, vec3(0.299, 0.587, 0.114))));	//vec3(0.2126, 0.7152, 0.0722)))));
}