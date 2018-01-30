#pragma once

#include "Shader.h"

class Pass
{
public:
	Pass(void);
	~Pass(void);

private:
	Shader* m_shader;
	GLuint	m_textureID[3];
	GLuint	m_targetFrameBuffer;
	float	m_otherData;

public:
	void	SetShader(Shader* shader)				{ m_shader = shader; }
	void	SetTexture(GLuint textureID, int index) { m_textureID[index] = textureID; }
	void	SetOtherData(float data)				{ m_otherData = data; }
	void	SetTarget(GLuint target);

	void	Render();
};