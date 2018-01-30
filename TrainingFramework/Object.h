#pragma once

#include "Commons.h"

class Object
{
public:
	int			m_id;
	int			m_modelID;
	int			m_shaderID;
	int			m_numTexture;
	int*		m_texturesID;
	int			m_numCubeTexture;
	int*		m_cubeTexturesID;
	int			m_numLight;
	int*		m_lightsID;
	Vector3		m_pos;
	Vector3		m_rotate;
	Vector3		m_scale;
public:
	~Object();
	void		SetPos   (float x, float y, float z) { m_pos.x    = x; m_pos.y    = y; m_pos.z    = z; }
	void		SetRotate(float x, float y, float z) { m_rotate.x = x; m_rotate.y = y; m_rotate.z = z; }
	void		SetScale (float x, float y, float z) { m_scale.x  = x; m_scale.y  = y; m_scale.z  = z; }
	void		Log();
	void		Render();
	void		RenderNormalTangentBitangent();
};