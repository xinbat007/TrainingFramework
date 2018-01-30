uniform		mediump mat4 u_mvpMatrix;
uniform		mediump mat4 u_maModel;
uniform		mediump mat4 u_maViewModel;

uniform		vec3 light_position;
uniform		vec4 light_ambient;
uniform		vec4 light_diffuse;
uniform		float light_constantAttenuation;
uniform		float light_linearAttenuation;
uniform		float light_quadraticAttenuation;

attribute	vec3 a_posL;
attribute	vec3 a_normL;
attribute	vec2 a_uv;

varying		vec4 v_ambient;
varying		vec4 v_diffuse;
varying		vec2 v_uv;

void main()
{
	gl_Position = u_mvpMatrix * vec4(a_posL, 1.0);
	v_uv = a_uv;
	
	vec3 currentVertexPos = (u_maViewModel * vec4(a_posL, 0.0)).xyz;
	float distance = length(light_position - currentVertexPos);
	
	vec4 normalVector = normalize(u_maModel * vec4(a_normL, 0.0));
	vec4 currentLightVector = normalize(vec4(light_position - currentVertexPos, 0.0));
	float NDotL = max(0.0, dot(normalVector, currentLightVector));
	float attenuation = 1.0 /  (light_constantAttenuation +
								light_linearAttenuation * distance + 
								light_quadraticAttenuation * distance * distance);
	v_diffuse = light_diffuse * NDotL * attenuation;
	v_ambient = light_ambient;
	
	
	/*vec4 lightVector = normalize(vec4(light_position, 0.0));
	float NDotL = max(0.0, dot(normalVector, lightVector));
	v_diffuse = light_diffuse * NDotL;
	v_ambient = light_ambient;*/
}