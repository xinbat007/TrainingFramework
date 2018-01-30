#include <stdafx.h>
#include "Model.h"
////////////////////////////////////////////////////////////////////////
Model::Model()
{
	m_vboID[0] = m_vboID[1] = m_vboID[2] = m_vboID[3] = m_vboID[4] = -1;
	vertices		= NULL;
	indicates		= NULL;
	m_terrainHeight = NULL;
	m_isSetNormal	= false;
}
////////////////////////////////////////////////////////////////////////
Model::~Model()
{
	//LOGI("Model: del NFG data id: %d", m_id);
	m_numVertice = 0;
	m_numIndicate = 0;
	glDeleteBuffers(5, m_vboID);
	SAFE_DEL(m_terrainHeight);
	SAFE_DEL_ARRAY(m_nfgFile);
	SAFE_DEL_ARRAY(m_heightMapFile);
	SAFE_DEL_ARRAY(indicates);
	SAFE_DEL_ARRAY(vertices);
};
////////////////////////////////////////////////////////////////////////
bool Model::LoadModelFromFile(const char* nfgDir, const char* heightMapDir)
{
	char nfgfile[200], heightMapFile[200];
#ifdef WIN32
	sprintf(nfgfile, "%s", nfgDir);
	sprintf(heightMapFile, "%s", heightMapDir);
#else
	sprintf(nfgfile, "%s/Resources/%s", SD_FOLDER, nfgDir);
	if (strcmp(heightMapDir, ""))
		sprintf(heightMapFile, "");
	else
		sprintf(heightMapFile, "%s/Resources/%s", SD_FOLDER, heightMapDir);
#endif
	m_nfgFile		= new char[strlen(nfgfile)];
	m_heightMapFile = new char[strlen(heightMapFile)];
	strcpy(m_nfgFile, nfgfile);
	strcpy(m_heightMapFile, heightMapFile);
	LOGI("AAAA: model: %s, %d, %s", m_nfgFile, strlen(m_nfgFile), m_heightMapFile);
	//------------------------------------
	m_terrainHeight = new TerrainHeight();
	if (!m_terrainHeight->LoadDataFromFile(m_heightMapFile))
	{
		SAFE_DEL(m_terrainHeight);
		m_terrainHeight = NULL;
	}
	//------------------------------------
	FILE*	pFile;
	char	buff[LINE_LENGTH];
	pFile = fopen(m_nfgFile, "r");
	if (!pFile)
	{
		fputs("FILE NFG ERROR", stderr);
		return false;
	}
	READ_LINE();
	sscanf(buff, "NrVertices: %d", &m_numVertice);
	SAFE_DEL_ARRAY(vertices);
	vertices = new Vertex[m_numVertice];
	for (int i = 0; i < m_numVertice; i++)
	{
		READ_LINE();
		sscanf(buff, "%*d. \
					 pos:[%f, %f, %f]; \
					 norm:[%f, %f, %f]; \
					 binorm:[%f, %f, %f]; \
					 tgt:[%f, %f, %f]; \
					 uv:[%f, %f];",
					 &vertices[i].pos.x,      &vertices[i].pos.y,      &vertices[i].pos.z,
					 &vertices[i].normal.x,   &vertices[i].normal.y,   &vertices[i].normal.z,
					 &vertices[i].binormal.x, &vertices[i].binormal.y, &vertices[i].binormal.z,
					 &vertices[i].tangent.x,  &vertices[i].tangent.y,  &vertices[i].tangent.z,
					 &vertices[i].uv.x,       &vertices[i].uv.y);
	}
	READ_LINE();
	sscanf(buff, "NrIndices: %d", &m_numIndicate);
	indicates = new unsigned short[m_numIndicate];
	for (int i = 0; i < m_numIndicate / 3; i++)
	{
		READ_LINE();
		sscanf(buff, "%*d. %u, %u, %u", &indicates[i * 3],
										&indicates[i * 3 + 1], 
										&indicates[i * 3 + 2]);
	}
	fclose (pFile);
	//-----------------------------------
	//Height map
	if (m_terrainHeight)
	{
		m_terrainHeight->SetModelSize(vertices, m_numVertice);
		for(int i = 0; i < m_numVertice; i++)
			vertices[i].pos.y = m_terrainHeight->GetHeightAt(vertices[i].pos, false);
	}
	//------------------------------------
	glGenBuffers(5, m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_numVertice, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//----
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * m_numIndicate, indicates, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
////////////////////////////////////////////////////////////////////////
void Model::SetNormalTangentBitangent(Vector3 scale)
{
	if (m_isSetNormal)
		return;
	m_isSetNormal = true;
	Vector3* vTemp = new Vector3[m_numVertice * 2];
	float lengthVector = 0.1f;
	//------------------------------------
	for (int i = 0; i < m_numVertice; i++)
	{
		vTemp[i * 2].x		= vertices[i].pos.x;
		vTemp[i * 2].y		= vertices[i].pos.y;
		vTemp[i * 2].z		= vertices[i].pos.z;
		vTemp[i * 2 + 1].x	= vertices[i].pos.x + vertices[i].normal.x / scale.x * lengthVector;
		vTemp[i * 2 + 1].y	= vertices[i].pos.y + vertices[i].normal.y / scale.y * lengthVector;
		vTemp[i * 2 + 1].z	= vertices[i].pos.z + vertices[i].normal.z / scale.z * lengthVector;
	}
	//----
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * m_numVertice * 2, vTemp, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//------------------------------------
	for (int i = 0; i < m_numVertice; i++)
	{
		vTemp[i * 2 + 1].x	= vertices[i].pos.x + vertices[i].tangent.x / scale.x * lengthVector;
		vTemp[i * 2 + 1].y	= vertices[i].pos.y + vertices[i].tangent.y / scale.y * lengthVector;
		vTemp[i * 2 + 1].z	= vertices[i].pos.z + vertices[i].tangent.z / scale.z * lengthVector;
	}
	//----
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * m_numVertice * 2, vTemp, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//------------------------------------
	for (int i = 0; i < m_numVertice; i++)
	{
		vTemp[i * 2 + 1].x	= vertices[i].pos.x + vertices[i].binormal.x / scale.x * lengthVector;
		vTemp[i * 2 + 1].y	= vertices[i].pos.y + vertices[i].binormal.y / scale.y * lengthVector;
		vTemp[i * 2 + 1].z	= vertices[i].pos.z + vertices[i].binormal.z / scale.z * lengthVector;
	}
	//----
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * m_numVertice * 2, vTemp, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//------------------------------------
	SAFE_DEL_ARRAY(vTemp);
	vTemp = NULL;
}
////////////////////////////////////////////////////////////////////////
void Model::Log()
{
	LOGI("Model %d: %s, %s", m_id, m_nfgFile, m_heightMapFile);
}
////////////////////////////////////////////////////////////////////////
void Model::ToString()
{
	for (int i = 0; i < m_numVertice; i++)
	{
		printf("AAAA: %d: %f, %f, %f\n", i, vertices[i].pos.x,
											vertices[i].pos.y,
											vertices[i].pos.z);
	}
	for (int i = 0; i < m_numVertice; i++)
	{
		printf("AAAA: %d: %f, %f\n", i, vertices[i].uv.x,
										vertices[i].uv.y);
	}
	for (int i = 0; i < m_numIndicate / 3; i++)
	{
		printf("AAAA: %d: %d, %d, %d\n", i, indicates[i * 3 + 0],
											indicates[i * 3 + 1],
											indicates[i * 3 + 2]);
	}
};
////////////////////////////////////////////////////////////////////////