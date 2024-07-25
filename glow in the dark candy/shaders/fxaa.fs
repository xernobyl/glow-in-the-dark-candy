#version 330

#ifndef FXAA_GATHER4_ALPHA
#ifdef GL_ARB_texture_gather
#extension GL_ARB_texture_gather : enable
#extension GL_ARB_texture_gather
#define FXAA_GATHER4_ALPHA 1
#else
#define FXAA_GATHER4_ALPHA 0
#endif
#endif

out vec3 O0;

uniform sampler2DRect S;
uniform vec2 s;

#ifndef FXAA_QUALITY__EDGE_THRESHOLD		// The minimum amount of local contrast required to apply algorithm.
	// 1/3 - too little
	// 1/4 - low quality
	// 1/6 - default
	// 1/8 - high quality
	// 1/16 - overkill
	#define FXAA_QUALITY__EDGE_THRESHOLD (1.0/6.0)
#endif

#ifndef FXAA_QUALITY__EDGE_THRESHOLD_MIN	// Trims the algorithm from processing darks.
	// 1/32 - visible limit
	// 1/16 - high quality
	// 1/12 - upper limit (default, the start of visible unfiltered edges)
	#define FXAA_QUALITY__EDGE_THRESHOLD_MIN (1.0/12.0)
#endif

#ifndef FXAA_QUALITY__SUBPIX_CAP			// Insures fine detail is not completely removed, this partly overrides FXAA_SUBPIX_TRIM.
	// 3/4 - default amount of filtering
	// 7/8 - high amount of filtering
	// 1 - no capping of filtering
	#define FXAA_QUALITY__SUBPIX_CAP (3.0/4.0)
#endif

#ifndef FXAA_QUALITY__SUBPIX_TRIM			// Controls removal of sub-pixel aliasing,
	// 1/2 - low removal (sharper but more sub-pixel aliasing)
	// 1/3 - medium removal
	// 1/4 - default removal
	// 1/8 - high removal
	// 0 - complete removal (softer but less sub-pixel aliasing)
	#define FXAA_QUALITY__SUBPIX_TRIM (1.0/4.0)
#endif


vec3 FxaaPixelShader(vec2 pos)
{   
	#if(FXAA_GATHER4_ALPHA == 1)
		vec4 luma4A = textureGatherOffset(S, pos, ivec2(-1, -1));
		vec4 rgbyM = texture(S, pos);
		vec4 luma4B = textureGather(S, pos);
		float lumaNE = textureOffset(S, pos, ivec2(1, -1)).w;
		float lumaSW = textureOffset(S, pos, ivec2(-1, 1)).w;
		float lumaNW = luma4A.w;
		float lumaN  = luma4A.z;
		float lumaW  = luma4A.x;
		float lumaM  = luma4A.y;
		float lumaE  = luma4B.z;
		float lumaS  = luma4B.x;
		float lumaSE = luma4B.y;
	#else
		float lumaN = textureOffset(S, pos, ivec2(0, -1)).w;
		float lumaW = textureOffset(S, pos, ivec2(-1, 0)).w;
		vec4 rgbyM = texture(S, pos);
		float lumaE = textureOffset(S, pos, ivec2( 1, 0)).w;
		float lumaS = textureOffset(S, pos, ivec2( 0, 1)).w;
		float lumaM = rgbyM.w;
	#endif

	float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
	float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
	float range = rangeMax - rangeMin;

	if(range < max(FXAA_QUALITY__EDGE_THRESHOLD_MIN, rangeMax * FXAA_QUALITY__EDGE_THRESHOLD))
		return rgbyM.rgb;

	#if(FXAA_GATHER4_ALPHA == 0) 
		float lumaNW = textureOffset(S, pos, ivec2(-1,-1)).w;
		float lumaNE = textureOffset(S, pos, ivec2( 1,-1)).w;
		float lumaSW = textureOffset(S, pos, ivec2(-1, 1)).w;
		float lumaSE = textureOffset(S, pos, ivec2( 1, 1)).w;
	#endif

	#define FXAA_QUALITY__SUBPIX_TRIM_SCALE  (1.0/(1.0 - FXAA_QUALITY__SUBPIX_TRIM))

	float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
	float rangeL = abs(lumaL - lumaM);
	float blendL = clamp((rangeL / range) - FXAA_QUALITY__SUBPIX_TRIM, 0.0, 1.0) * FXAA_QUALITY__SUBPIX_TRIM_SCALE; 
	blendL = min(FXAA_QUALITY__SUBPIX_CAP, blendL);

	float edgeVert = 
			  abs(lumaNW + (-2.0 * lumaN) + lumaNE) +
		2.0 * abs(lumaW  + (-2.0 * lumaM) + lumaE ) +
			  abs(lumaSW + (-2.0 * lumaS) + lumaSE);
	float edgeHorz = 
			  abs(lumaNW + (-2.0 * lumaW) + lumaSW) +
		2.0 * abs(lumaN  + (-2.0 * lumaM) + lumaS ) +
			  abs(lumaNE + (-2.0 * lumaE) + lumaSE);
	bool horzSpan = edgeHorz >= edgeVert;

	float lengthSign = -1.0;
	if(!horzSpan) lumaN = lumaW;
	if(!horzSpan) lumaS = lumaE;
	float gradientN = abs(lumaN - lumaM);
	float gradientS = abs(lumaS - lumaM);
	lumaN = (lumaN + lumaM) * 0.5;
	lumaS = (lumaS + lumaM) * 0.5;

	bool pairN = gradientN >= gradientS;
	if(!pairN) lumaN = lumaS;
	if(!pairN) gradientN = gradientS;
	if(!pairN) lengthSign *= -1.0;
	vec2 posN;
	posN.x = pos.x + (horzSpan ? 0.0 : lengthSign * 0.5);
	posN.y = pos.y + (horzSpan ? lengthSign * 0.5 : 0.0);

	#define FXAA_SEARCH_STEPS     6
	#define FXAA_SEARCH_THRESHOLD (1.0/4.0)

	gradientN *= FXAA_SEARCH_THRESHOLD;

	vec2 posP = posN;
	vec2 offNP = horzSpan ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	float lumaEndN;
	float lumaEndP;
	bool doneN = false;
	bool doneP = false;
	posN += offNP * (-1.5);
	posP += offNP * ( 1.5);

	for(int i = 0; i < FXAA_SEARCH_STEPS; i++)
	{
		lumaEndN = texture(S, posN.xy).w;
		lumaEndP = texture(S, posP.xy).w;
		bool doneN2 = abs(lumaEndN - lumaN) >= gradientN;
		bool doneP2 = abs(lumaEndP - lumaN) >= gradientN;
		if(doneN2 && !doneN)
			posN += offNP;
		if(doneP2 && !doneP)
			posP -= offNP;
		if(doneN2 && doneP2)
			break;
		doneN = doneN2;
		doneP = doneP2;
		if(!doneN)
			posN -= offNP * 2.0;
		if(!doneP)
			posP += offNP * 2.0;
	}

	float dstN = horzSpan ? pos.x - posN.x : pos.y - posN.y;
	float dstP = horzSpan ? posP.x - pos.x : posP.y - pos.y;

	bool directionN = dstN < dstP;
	lumaEndN = directionN ? lumaEndN : lumaEndP;

	if(((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
		lengthSign = 0.0;

	float spanLength = (dstP + dstN);
	dstN = directionN ? dstN : dstP;
	float subPixelOffset = 0.5 + (dstN * (-1.0/spanLength));
	subPixelOffset += blendL * (1.0/8.0);
	subPixelOffset *= lengthSign;

	vec3 rgbF = texture(S, vec2(
		pos.x + (horzSpan ? 0.0 : subPixelOffset),
		pos.y + (horzSpan ? subPixelOffset : 0.0))).xyz;

	lumaL *= lumaL;
	float lumaF = dot(rgbF, vec3(0.299, 0.587, 0.114)) + (1.0/(65536.0*256.0));
	float lumaB = mix(lumaF, lumaL, blendL);
	float scale = min(4.0, lumaB/lumaF);
	rgbF *= scale;

	return rgbF;
}


void main()
{
	O0 = FxaaPixelShader(gl_FragCoord.xy * s);
}