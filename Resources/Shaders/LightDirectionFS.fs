precision mediump float;

uniform sampler2D u_Texture1;

varying vec4 v_ambient;
varying vec4 v_diffuse;
varying vec2 v_uv;

void main()
{
	vec4 fragmentColor = texture2D(u_Texture1, v_uv);
	gl_FragColor = fragmentColor * (v_diffuse + v_ambient);
	//gl_FragColor = v_diffuse + v_ambient;
}