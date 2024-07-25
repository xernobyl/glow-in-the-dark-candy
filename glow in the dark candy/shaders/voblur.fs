#version 330

uniform sampler2DRect S;
uniform float t;

out vec2 O0;

float ws;
float as;

vec2 s[9];

void B(int i, float w, inout float ws, inout float as)
{
	float zw = w / (1.0 + t * abs(s[4].g - s[i].g) / s[4].g);
	ws += zw;
	as += zw * s[i].r;
}

void main()
{
	s[0] = texture(S, gl_FragCoord.xy + vec2(-231.0/682.0-1, -231.0/682.0-1)).rg;
	s[1] = texture(S, gl_FragCoord.xy + vec2(-251.0/506.0-1, 0.0)).rg;
	s[2] = texture(S, gl_FragCoord.xy + vec2(-231.0/682.0-1, 231.0/682.0+1)).rg;
	
	s[3] = texture(S, gl_FragCoord.xy + vec2(0.0, -251.0/506.0-1)).rg;
	s[4] = texture(S, gl_FragCoord.xy).rg;
	s[5] = texture(S, gl_FragCoord.xy + vec2(0.0, 251.0/506.0+1)).rg;
	
	s[6] = texture(S, gl_FragCoord.xy + vec2(231.0/682.0+1, -231.0/682.0-1)).rg;
	s[7] = texture(S, gl_FragCoord.xy + vec2(251.0/506.0+1, 0.0)).rg;
	s[8] = texture(S, gl_FragCoord.xy + vec2(231.0/682.0+1, 231.0/682.0+1)).rg;

	ws = 0.0;
	as = 0.0;

	B(0, 682.0/255.0, ws, as);
	B(1, 506.0/255.0, ws, as);
	B(2, 682.0/255.0, ws, as);

	B(3, 506.0/255.0, ws, as);
	B(4, 1.0, ws, as);
	B(5, 506.0/255.0, ws, as);

	B(6, 682.0/255.0, ws, as);
	B(7, 506.0/255.0, ws, as);
	B(8, 682.0/255.0, ws, as);

	O0 = s[4];//vec2(clamp(as/ws, 0.0, 1.0), s[4].g);
}