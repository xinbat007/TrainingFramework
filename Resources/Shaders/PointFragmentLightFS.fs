precision mediump float;

uniform sampler2D u_Texture1;

uniform		vec3 u_cameraPos;
uniform		vec4 light_ambient;
uniform		vec4 light_position;
uniform		float u_ambientWeight;
uniform		float light_constantAttenuation;
uniform		float light_linearAttenuation;

varying vec4 v_vertexW;
varying	vec4 v_normalW;
varying vec2 v_uv;

void main()
{
	vec4 colorFragment = texture2D(u_Texture1, v_uv);
	vec4 ambientComponent = light_ambient;
	
	vec4 lightDirection	= light_position - v_vertexW;
	
	float distance = length(lightDirection);
	float attenuation = clamp(1.0 / (light_constantAttenuation * distance * distance +
									light_linearAttenuation), 0.0, 1.0);
	vec4 lightDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 diffuseComponent = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularComponent = vec4(0.0, 0.0, 0.0, 0.0);
	diffuseComponent = max(dot(v_normalW, lightDirection), 0.0) * lightDiffuse * attenuation;
	vec4 toEye = normalize(vec4(u_cameraPos, 0.0) - v_vertexW);
	vec4 reflectVector = normalize(-reflect(lightDirection, v_normalW));
	specularComponent = pow(max(dot(reflectVector, toEye), 0.0), 10.0) * lightSpecular * attenuation;
	
	gl_FragColor = vec4(( (ambientComponent * u_ambientWeight + diffuseComponent * (1.0 - u_ambientWeight)) * 
							colorFragment + specularComponent).xyz, colorFragment.w);
	
}