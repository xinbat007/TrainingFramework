#include <stdafx.h>
#include "Texture.h"
#ifdef WIN32
#else
#include "../GameSpecific/jni/config_Android.h"
#endif
////////////////////////////////////////////////////////////////////////
Texture::~Texture()
{
	glDeleteBuffers(1, &m_vboID);
	SAFE_DEL_ARRAY(m_FileDir);
}
////////////////////////////////////////////////////////////////////////
char* Texture::ExtractCubeMap(char* imageBuffer, int width, int height, int bpp, int index)
{
	int w = width > height ? width / 4 : width / 3;
	int h = width > height ? height / 3 : height / 4;
	int d = bpp / 8;
	
	char* extractData = new char[w * h * d];
	memset(extractData, 200, w * h * d);
	
	char* offset;
	if (width > height)
	{
		switch(index)
		{
		case 0: // X+
			offset = &imageBuffer[h * width * d + 2 * w * d];
			break;
		case 1: // X-
			offset = &imageBuffer[h * width * d];
			break;
		case 2: // Y+
			offset = &imageBuffer[w * d];
			break;
		case 3: // Y-
			offset = &imageBuffer[2 * h * width * d + w * d];
			break;
		case 4: // Z+
			offset = &imageBuffer[h * width * d + w * d];
			break;
		case 5: // Z-
			offset = &imageBuffer[h * width * d + 3 * w * d];
			break;
		}
	}
	else
	{
		switch(index)
		{
		case 0: // X+
			offset = &imageBuffer[2 * h * width * d + w * d];
			break;
		case 1: // X-
			offset = &imageBuffer[w * d];
			break;
		case 2: // Y+
			offset = &imageBuffer[h * width * d + 2 * w * d];
			break;
		case 3: // Y-
			offset = &imageBuffer[h * width * d];
			break;
		case 4: // Z+
			offset = &imageBuffer[h * width * d + w * d];
			break;
		case 5: // Z-
			offset = &imageBuffer[3 * h * width * d + w * d];
			break;
		}
	}
	for (int i = 0 ; i < h; i++)
		memcpy(extractData + i * w * d, offset + i * width * d, w * d);
	return extractData;
}
////////////////////////////////////////////////////////////////////////
void Texture::LoadTextureFromFile(const char* szfileName)
{
	int iWidth;
	int iHeight;
	int bpp;
	char *imageBuffer = NULL;
	char fileName[200];
#ifdef WIN32
	sprintf(fileName, "%s", szfileName);
#else
	sprintf(fileName, "%s/Resources/%s", SD_FOLDER, szfileName);
#endif
	m_FileDir = new char[strlen(fileName)];
	strcpy(m_FileDir, fileName);
	//LOGI("AAAA: load texture: %s, %d, %d", m_FileDir, strlen(m_FileDir), m_isCube);
	if (m_isCube)
	{
		if (strstr(m_FileDir, ".tga"))
		{
			glGenTextures(1, &m_vboID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_vboID);
			imageBuffer = LoadTGA(m_FileDir, &iWidth, &iHeight, &bpp);
			for (int i = 0; i < 6; i++)
			{
				char* extractData = ExtractCubeMap(imageBuffer, iWidth, iHeight, bpp, i);
				glTexImage2D (	GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
								bpp == 24 ? GL_RGB : GL_RGBA,
								iWidth > iHeight ? iWidth / 4 : iWidth / 3,
								iWidth > iHeight ? iHeight / 3 : iHeight / 4,
								0,
								bpp == 24 ? GL_RGB : GL_RGBA,
								GL_UNSIGNED_BYTE, extractData);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				SAFE_DEL_ARRAY(extractData);
			}
			SAFE_DEL_ARRAY(imageBuffer);
		}
		else
		{
			char* path[6];
			for (int i = 0; i < 6; i++)
			{
				path[i] = new char[MAX_LENGTH];
				strcpy(path[i], m_FileDir);
			}
			strcat(path[0], "right.tga");
			strcat(path[1], "left.tga");
			strcat(path[2], "top.tga");
			strcat(path[3], "bottom.tga");
			strcat(path[4], "front.tga");
			strcat(path[5], "back.tga");
			//------------------------------
			glGenTextures(1, &m_vboID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_vboID);
			for (int i = 0; i < 6; i++)
			{
				//LOGI("AAAA: load cube: %d, %s", i, path[i]);
				imageBuffer = LoadTGA(path[i], &iWidth, &iHeight, &bpp);
				glTexImage2D (	GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
								bpp == 24 ? GL_RGB : GL_RGBA, iWidth, iHeight, 0,
								bpp == 24 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				SAFE_DEL_ARRAY(imageBuffer);
			}
			//------------------------------
			for (int i = 0; i < 6; i++)
			{
				SAFE_DEL_ARRAY(path[i]);
			}
		}
	}
	else
	{
		imageBuffer = LoadTGA(m_FileDir, &iWidth, &iHeight, &bpp);
		//LOGI("AAAA: image: %d - %d, %d", iWidth, iHeight, bpp);
		glGenTextures(1, &m_vboID);
		glBindTexture(GL_TEXTURE_2D, m_vboID);
		glTexImage2D(GL_TEXTURE_2D, 0, bpp == 24 ? GL_RGB : GL_RGBA, iWidth, iHeight, 0,
									   bpp == 24 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SAFE_DEL_ARRAY(imageBuffer);
	}
}
////////////////////////////////////////////////////////////////////////
void Texture::ApplyTiling()
{
	if (m_isCube)
	{
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
}
////////////////////////////////////////////////////////////////////////
void Texture::SetMinFilter(const char* minFilter)
{
	if (!strcmp(minFilter, "NONE"))
	{
		m_minFilter = 0;
	}
	else if (!strcmp(minFilter, "NEAREST_MIPMAP_NEAREST"))
	{
		m_minFilter = GL_NEAREST_MIPMAP_NEAREST;
	}
	else if (!strcmp(minFilter, "NEAREST_MIPMAP_LINEAR"))
	{
		m_minFilter = GL_NEAREST_MIPMAP_LINEAR;
	}
	else if (!strcmp(minFilter, "LINEAR_MIPMAP_NEAREST"))
	{
		m_minFilter = GL_LINEAR_MIPMAP_NEAREST;
	}
	else if (!strcmp(minFilter, "LINEAR_MIPMAP_LINEAR"))
	{
		m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
	}
	else
	{
		LOGI("AAAA: COULD NOT FOUND MINFILTER: %s", minFilter);
		m_minFilter = 0;
	}
}
////////////////////////////////////////////////////////////////////////
void Texture::SetMagFilter(const char* magFilter)
{
	if (!strcmp(magFilter, "NONE"))
	{
		m_magFilter = 0;
	}
	else if (!strcmp(magFilter, "LINEAR"))
	{
		m_magFilter = GL_LINEAR;
	}
	else if (!strcmp(magFilter, "NEAREST"))
	{
		m_magFilter = GL_NEAREST;
	}
	else
	{
		LOGI("AAAA: COULD NOT FOUND MAGFILTER: %s", magFilter);
		m_magFilter = 0;
	}
}
////////////////////////////////////////////////////////////////////////
void Texture::SetTiling(const char* tiling)
{
	if (!strcmp(tiling, "REPEAT"))
	{
		m_tiling = GL_REPEAT;
	}
	else if (!strcmp(tiling, "CLAMP"))
	{
		m_tiling = GL_CLAMP_TO_EDGE;
	}
	else
	{
		LOGI("AAAA: COULD NOT FOUND TILING: %s", tiling);
		m_tiling = 0;
	}
	ApplyTiling();
}
////////////////////////////////////////////////////////////////////////
void Texture::Log()
{
	LOGI("Texture: %d, %s", m_ID, m_FileDir/*, m_minFilter, m_magFilter, m_tiling*/);
}
////////////////////////////////////////////////////////////////////////
