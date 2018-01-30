precision mediump float;

uniform sampler2D	u_BlendMap;
uniform sampler2D	u_Texture1;
uniform sampler2D	u_Texture2;
uniform sampler2D	u_Texture3;

uniform float		u_fogStart;
uniform float		u_fogLength;

varying float		v_eyeDist;
varying vec2		v_uv;

void main()
{
	vec4 BT   = texture2D(u_BlendMap, v_uv);
	vec4 Tex1 = texture2D(u_Texture1, v_uv * 10.0);
	vec4 Tex2 = texture2D(u_Texture2, v_uv * 10.0);
	vec4 Tex3 = texture2D(u_Texture3, v_uv * 10.0);

	float lerpValue = clamp((v_eyeDist - u_fogStart) / u_fogLength, 0.0, 1.0);
	vec4 fogColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	vec4 FinalColor = lerpValue * fogColor + (1.0 - lerpValue) * (BT.r * Tex1 + BT.g * Tex2 + BT.b * Tex3);
	
	gl_FragColor = FinalColor;
}