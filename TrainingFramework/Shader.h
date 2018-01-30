#pragma once
#include "../Utilities/utilities.h"
#include "Defines.h"

class Shader 
{
private:
	int			m_id;
	char*		m_VSFile;
	char*		m_FSFile;
	int			m_result;
	GLuint LoadShader ( GLenum type, char * filename );
	GLuint LoadProgram ( GLuint vertexShader, GLuint fragmentShader );
public:
	GLuint		program, vertexShader, fragmentShader;
	// Texture
	GLint		positionAttribute, uiTextureAttribute, colorAttribute, mvpAttribute;
	// Terrain
	GLint		m_locBlendMap, m_locTexture1, m_locTexture2, m_locTexture3;
	// Fog
	GLint		m_fogStart, m_fogLength, m_modelMatrix, m_cameraPos;
	// Fire
	GLint		m_time;
	// Sky
	GLint		m_cubeUniform;
	// Reflection
	GLint		m_normL, m_worldMatrix;
	// Direction Lighting
	GLint		m_maModel, m_lightPosition, m_lightAmbient, m_lightDiffuse;
	// Point Lighting
	GLint		m_maViewModel, m_lightConstantAtn, m_lightLinearAtn, m_lightQuadraticAtn;
	// Point Specular Lighting
	GLint		m_lightSpecular, m_materialShininess;
	// Point Fragment Lighting
	GLint		m_ambientWeight;
	// Normal Mapping
	GLint		m_tangentL, m_bitangentL, m_textureNormalMap;
	// Blur
	GLint		m_step;
	// Bloom
	GLint		m_limit;
	// Depth of Field
	GLint		m_near, m_far, m_fade, m_clarity;

	int			Load(char* fileVertexShader, char* fileFragmentShader);
	int			GetID() { return m_id; }
	void		SetID(int id) { m_id = id; }
	int			GetResult() { return m_result; }
	void		SetResult(int res) { m_result = res; }
	void		Log();
	~Shader();
};