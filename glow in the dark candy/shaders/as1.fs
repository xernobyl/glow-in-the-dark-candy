#version 330

uniform sampler2DRect A, Z;
uniform vec2 zc;

const float h = 4.88e-04;
const float o[5] = float[](0.0, 1.0 * 98.0/68.0, 1.0 * 141.0/41.0, 1.0 * 130.0/24.0, 1.0 * 66.0/9.0);
const float w[5] = float[](51.0/335.0, 68.0/335.0, 41.0/335.0, 24.0/335.0, 9.0/335.0);

out vec4 O0;

void main()
{
	vec4 t = texture(A, gl_FragCoord.xy * 0.5);
	float zl = t.y + t.z / 256.0 + t.w / 65536.0;

	float zh = texture(Z, gl_FragCoord.xy).r;

	float tw = w[0];
	float a = t.r * w[0];

	for(int i=1; i<5; i++)
	{
		vec4 t0 = texture(A, 0.5 * (gl_FragCoord.xy + vec2(o[i], o[i])));
		float z0 = t0.y + t0.z / 256.0 + t0.w / 65536.0;

		vec4 t1 = texture(A, 0.5 * (gl_FragCoord.xy - vec2(o[i], o[i])));
		float z1 = t1.y + t1.z / 256.0 + t1.w / 65536.0;

		float w0 = w[i] / (h + abs(zh - z0));
		float w1 = w[i] / (h + abs(zh - z1));

		tw += w0 + w1;
		a += t0.r * w0 + t1.r * w1;
	}

	float u = floor(zh * 256.0);
	float res_u = zh * 256.0 - u;
	float v = floor(res_u * 256.0);

	O0 = vec4(a / tw, vec2(u,v) / 256.0, (zh - zl) / 0.01 + 0.5);
}