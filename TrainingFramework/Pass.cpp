#include "stdafx.h"
#include "Pass.h"
#include "EffectManager.h"
#include "SceneManager.h"
////////////////////////////////////////////////////////////////////////
Pass::Pass(void)
{
	m_textureID[0] = m_textureID[1] = m_textureID[2] = -1;
}
////////////////////////////////////////////////////////////////////////
Pass::~Pass(void)
{
}
////////////////////////////////////////////////////////////////////////
void Pass::Render()
{
	//LOGI("AAAA: target: %d", m_targetFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_targetFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_shader->program);
	//---------------
	if (m_shader->positionAttribute != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, EffectManager::GetInstance()->GetQuadVBOId());
		glEnableVertexAttribArray(m_shader->positionAttribute);
		glVertexAttribPointer(m_shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//----- Texture
	int activeTextureIndex = 0;
	if (m_shader->m_locTexture1 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + activeTextureIndex);
		glBindTexture(GL_TEXTURE_2D, m_textureID[0]);
		glUniform1i(m_shader->m_locTexture1, activeTextureIndex);
		activeTextureIndex++;
	}
	if (m_shader->m_locTexture2 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + activeTextureIndex);
		glBindTexture(GL_TEXTURE_2D, m_textureID[1]);
		glUniform1i(m_shader->m_locTexture2, activeTextureIndex);
		activeTextureIndex++;
	}
	if (m_shader->m_locTexture3 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + activeTextureIndex);
		glBindTexture(GL_TEXTURE_2D, m_textureID[2]);
		glUniform1i(m_shader->m_locTexture3, activeTextureIndex);
		activeTextureIndex++;
	}
	//----- Blur
	if (m_shader->m_step != -1)
	{
		float x = 1.0f / g_screenWidth;
		float y = 1.0f / g_screenHeight;
		float z = sqrt(2.0f) / 2.0f * x;
		float w = sqrt(2.0f) / 2.0f * y;
		glUniform4f(m_shader->m_step, m_otherData * x, m_otherData * y, m_otherData * z, m_otherData * w);
	}
	//----- Bloom
	if (m_shader->m_limit != -1)
	{
		glUniform1f(m_shader->m_limit, m_otherData);
	}
	//----- DOF
	if (m_shader->m_near != -1)
	{
		glUniform1f(m_shader->m_near, SceneManager::GetInstance()->GetActiveCamera()->GetNearPlane());
	}
	if (m_shader->m_far != -1)
	{
		glUniform1f(m_shader->m_far, SceneManager::GetInstance()->GetActiveCamera()->GetFarPlane());
	}
	if (m_shader->m_fade != -1)
	{
		glUniform1f(m_shader->m_fade, 20.0f);
	}
	if (m_shader->m_clarity != -1)
	{
		glUniform1f(m_shader->m_clarity, 20.0f);
	}
	//---------------
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
////////////////////////////////////////////////////////////////////////
void Pass::SetTarget(GLuint target)
{
	if (target <= 0)
		m_targetFrameBuffer = 0;
	else
		m_targetFrameBuffer = EffectManager::GetInstance()->GetFBOIdAt(target - 1);
	LOGI("AAAA: SET TARGET: %d, %d", target, m_targetFrameBuffer);
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////