#pragma once

#include "Commons.h"

class Texture
{
private:
	int			m_ID;
	char*		m_FileDir;
	int			m_width;
	int			m_height;
	int			m_bpp;
	GLint		m_minFilter;
	GLint		m_magFilter;
	GLint		m_tiling;
	bool		m_isCube;
	GLuint		m_vboID;
	char*		ExtractCubeMap(char* imageBuffer, int width, int height, int bpp, int i);
public:
	void		LoadTextureFromFile(const char* fileName);
	void		SetID(int id) { m_ID = id; }
	int			GetID() { return m_ID; }
	void		SetMinFilter(const char* minFilter);
	void		SetMagFilter(const char* magFilter);
	void		SetTiling(const char* tiling);
	void		ApplyTiling();
	void		SetIsCubeTexture(bool b) { m_isCube = b; };
	GLuint		GetVBOId() { return m_vboID; };
	void		Log();
	~Texture();
};