#include <stdafx.h>
#include "TerrainHeight.h"
////////////////////////////////////////////////////////////////////////
TerrainHeight::TerrainHeight()
{
	m_data = NULL;
	m_fScale = 1.0f;
}
////////////////////////////////////////////////////////////////////////
TerrainHeight::~TerrainHeight()
{
	SAFE_DEL_ARRAY(m_data);
	m_data = NULL;
}
////////////////////////////////////////////////////////////////////////
bool TerrainHeight::LoadDataFromFile(const char* fileName)
{
	if (strlen(fileName) > 1)
	{
		m_isRaw = true;
		FILE* pFile = fopen (fileName , "r");
		if (pFile)
		{   
			fseek( pFile, 0, SEEK_END );
			int length = ftell( pFile );
			fseek( pFile, 0, SEEK_SET );

			m_data = new unsigned char[length];
			fread( m_data, 1, length, pFile );

			m_iWidth = m_iHeight = (int) sqrt(float(length));
			fclose( pFile );

			return true;
		}
		else
		{
			LOGI("Error opening TERRAIN file: %s",  fileName);
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////
void TerrainHeight::SetModelSize(Vertex* vertices, int n)
{
	minX = maxX = vertices[0].pos.x;
	minZ = maxZ = vertices[0].pos.z;

	for (int i = 0; i < n; i++)
	{
		if (minX > vertices[i].pos.x)
			minX = vertices[i].pos.x;
		if (maxX < vertices[i].pos.x)
			maxX = vertices[i].pos.x;
		if (minZ < vertices[i].pos.z)
			minZ = vertices[i].pos.z;
		if (maxZ > vertices[i].pos.z)
			maxZ = vertices[i].pos.z;
	}
}
////////////////////////////////////////////////////////////////////////
float TerrainHeight::GetHeightAt(Vector2 position)
{
	if (position.x < 0)
		position.x = 0;
	if (position.x > 1)
		position.x = 1;
	if (position.y < 0)
		position.y = 0;
	if (position.y > 1)
		position.y = 1;
	int x, y;
	if (m_isRaw)
	{
		x = int(position.x*(m_iWidth - 1));
		y = int(position.y*(m_iHeight - 1));
	}
	else
	{
		//rotate -90 degree
		x = int((1 - position.y)*(m_iWidth - 1));
		y = int((position.x)*(m_iHeight - 1));
	}

	//smoothing the height
	float mean;
	/*if (x > 0 && y > 0 && x < m_iWidth - 1 && y < m_iHeight - 1)
		mean = (m_data[x + m_iWidth * y] + 

				m_data[x + 1 + m_iWidth * y] + 
				m_data[x - 1 + m_iWidth * y]+ 
				m_data[x + m_iWidth * (y + 1)] + 
				m_data[x + m_iWidth * (y - 1)] +

				m_data[x + 1 + m_iWidth * (y + 1)] +
				m_data[x + 1 + m_iWidth * (y - 1)] +
				m_data[x - 1 + m_iWidth * (y - 1)] + 
				m_data[x - 1 + m_iWidth * (y + 1)]
				) / 9.0f;
	else*/
		mean = m_data[x + m_iWidth * y];

	return mean / 255.0f * 20.0f;//TERRAIN_PEAK_VALUE;

}
////////////////////////////////////////////////////////////////////////
float TerrainHeight::GetHeightAt(Vector3 position, bool isWorld)
{
	Vector4 vec4Pos;
	vec4Pos = Vector4(position.x, position.y, position.z, 1.0f);
	//LOGI("AAAA: posy: %f", position.y);
	/*if (isWorld)
	{
		vec4Pos = vec4Pos * m_matModel;
		return m_fScale * GetHeightAt(Vector2((vec4Pos.x - minX) / (maxX - minX), (vec4Pos.z - minY) / (maxY - minY)));
	}*/
	return GetHeightAt(Vector2(( vec4Pos.x - minX) / (maxX - minX),
								(vec4Pos.z - minZ) / (maxZ - minZ)));
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
