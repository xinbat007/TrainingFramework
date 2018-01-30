precision mediump float;

uniform sampler2D u_Texture1;

varying vec2 v_uv;

void main()
{
	//gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	gl_FragColor = texture2D(u_Texture1, v_uv);
}