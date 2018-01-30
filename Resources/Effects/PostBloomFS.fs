precision mediump float;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

varying mediump vec2 v_uv;

void main(void)
{
	vec4 color = texture2D(u_Texture1, v_uv);
	vec4 blur = texture2D(u_Texture2, v_uv);	
	gl_FragColor = color + 1.2 * blur;
}