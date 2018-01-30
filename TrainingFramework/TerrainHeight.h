#pragma once

#include "Commons.h"

class TerrainHeight
{
private:
	int				m_iWidth;
	int				m_iHeight;
	unsigned char*	m_data;
	bool			m_isRaw;

	float			minX, minZ, maxX, maxZ;
	float			m_fScale;

	float			GetHeightAt(Vector2 position);
public:
	TerrainHeight();
	~TerrainHeight();
	bool			LoadDataFromFile(const char* fileName);
	void			SetModelSize(Vertex* vertices, int n);
	float			GetHeightAt(Vector3 position, bool isWorld);
};