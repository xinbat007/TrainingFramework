precision mediump float;

uniform sampler2D u_Texture1;

varying vec2 v_uv;

void main()
{
	vec4 color = texture2D(u_Texture1, v_uv);
	float c = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	gl_FragColor = vec4(c, c, c, color.a);
}