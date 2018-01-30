precision mediump float;

uniform sampler2D	u_BlendMap;	// Fire texture
uniform sampler2D	u_Texture1;	// Fire Mask
uniform sampler2D	u_Texture2;	// Displacement
uniform float		u_Time;

varying vec2 v_uv;

void main(void)
{
	float dMax = 0.1;
	vec2 disp = texture2D(u_Texture2, vec2(v_uv.x, v_uv.y + u_Time)).rg;
	vec2 offset = (2.0 * disp - 1.0) * dMax;
	vec2 Tex_coords_displaced =	v_uv + offset;
	vec4 fire_color = texture2D(u_BlendMap, Tex_coords_displaced);
	vec4 AlphaValue = texture2D(u_Texture1, v_uv);
	gl_FragColor = fire_color * vec4(1.0, 1.0, 1.0, AlphaValue.x);
}