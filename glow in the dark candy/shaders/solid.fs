#version 330

out vec4 O0;

uniform sampler2DRect D, S;
uniform vec2 c;
uniform vec3 sc, dc;

void main()
{
	float coc = abs(gl_FragCoord.w * c.x + c.y);

	vec3 d = texture(D, gl_FragCoord.xy).rgb * 0.3183098862;
	vec3 s = texture(S, gl_FragCoord.xy).rgb * 0.3183098862;

	O0 = vec4(dc * d + sc * s, coc);
}