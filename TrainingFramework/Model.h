#pragma once

#include "Commons.h"
#include "TerrainHeight.h"

class Model
{
private:
	int					m_id;
	char*				m_nfgFile;
	char*				m_heightMapFile;
	TerrainHeight*		m_terrainHeight;
	bool				m_isSetNormal;
public:
	Model();
	~Model();
	int					m_numVertice, m_numIndicate;
	Vertex*				vertices;
	unsigned short*		indicates;
	GLuint				m_vboID[5];
	void				BindTexture(const char* textureDir);
	bool				LoadModelFromFile(const char* nfgDir, const char* heightMapDir);
	void				Log();
	void				ToString();
	void				SetID(int id) { m_id = id; }
	int					GetID() { return m_id; }
	char*				GetNFGFile() { return m_nfgFile; }
	char*				GetHeightMapFile() { return m_heightMapFile; }
	void				SetNormalTangentBitangent(Vector3 scale);
};

struct LightSource
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 position;
	Vector4 halfVector;
	Vector3 spotDirection;
	float spotExponent;
	float spotCutoff;
	float spotCosCutoff;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct Material
{
	Vector4 emission;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	float shininess;
};

struct LightModel
{
	Vector4 ambient;
};

struct LightProducts
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
};

struct LightModelProducts
{
	Vector4 sceneColor;
};