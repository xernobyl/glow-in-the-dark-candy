#version 330

out vec4 O0;

uniform sampler2DRect D, S;
uniform sampler2D A;
uniform vec2 c;

in vec2 t;

void main()
{
	float coc = abs(gl_FragCoord.w * c.x + c.y);

	vec3 d = texture(D, gl_FragCoord.xy).rgb * 0.3183098862;
	vec3 s = texture(S, gl_FragCoord.xy).rgb * 0.3183098862;

	O0 = vec4(texture(A, t).rgb * (d + s), coc);
}