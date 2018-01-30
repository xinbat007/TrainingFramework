precision mediump float;

uniform sampler2D u_Texture1;

uniform float u_limit;

varying mediump vec2 v_uv;

void main(void)
{
	vec3 color = texture2D(u_Texture1, v_uv).rgb;
	float brightness = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	float val = step(u_limit, brightness);
	gl_FragColor = vec4(color * val, 1.0);
}