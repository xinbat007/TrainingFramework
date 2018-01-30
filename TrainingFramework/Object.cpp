#include <stdafx.h>
#include "Object.h"
#include "Model.h"
#include "ResourceManager.h"
#include "SceneManager.h"
///////////////////////////////////////////////////
Object::~Object()
{
	SAFE_DEL_ARRAY(m_texturesID);
	SAFE_DEL_ARRAY(m_cubeTexturesID);
	SAFE_DEL_ARRAY(m_lightsID);
}
///////////////////////////////////////////////////
void Object::Log()
{
	LOGI("Object: %d, %d, %d, %d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
					m_id, m_modelID, m_shaderID, m_numTexture, m_numCubeTexture, m_numLight,
					m_pos.x, m_pos.y, m_pos.z,
					m_rotate.x, m_rotate.y, m_rotate.z,
					m_scale.x, m_scale.y, m_scale.z);
}
///////////////////////////////////////////////////
void Object::RenderNormalTangentBitangent()
{
	Shader* triangleShader = ResourceManager::GetInstance()->GetShaderById(0);
	glUseProgram(triangleShader->program);
	//--------------------------
	Matrix maModel;
	Camera* activeCamera = SceneManager::GetInstance()->GetActiveCamera();
	if (triangleShader->mvpAttribute != -1)
	{
		{
			Matrix maPos, maRot, maScale;
			maPos.SetTranslation(m_pos);
			maRot.SetRotationXYZ(m_rotate.x, m_rotate.y, m_rotate.z);
			maScale.SetScale(m_scale);
			maModel = maScale * maRot * maPos;
		}
		Matrix maMVPMatrix = maModel * SceneManager::GetInstance()->GetActiveCamera()->GetViewProjectMatrix();
		glUniformMatrix4fv (triangleShader->mvpAttribute, 1, GL_FALSE, (const GLfloat*)maMVPMatrix.m);
	}
	//-----------------------
	Model* nfgData = ResourceManager::GetInstance()->GetModelById(m_modelID);
	nfgData->SetNormalTangentBitangent(m_scale);
	//------
	unsigned short *indexes = new unsigned short[nfgData->m_numVertice * 2];
	for (int i = 0; i < nfgData->m_numVertice; i++)
	{
		indexes[i * 2]		= i * 2;
		indexes[i * 2 + 1]	= i * 2 + 1;
	}
	//------------------------------- // Normal - BLUE
	glBindBuffer(GL_ARRAY_BUFFER, nfgData->m_vboID[2]);
	if (triangleShader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(triangleShader->positionAttribute);
		glVertexAttribPointer(triangleShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (const void*)0);
	}
	//------
	if (triangleShader->colorAttribute != -1)
	{
		glUniform4f (triangleShader->colorAttribute, 0.0f, 0.0f, 1.0f, 1.0f);
	}
	glDrawElements(GL_LINES, nfgData->m_numVertice * 2, GL_UNSIGNED_SHORT, indexes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//------------------------------- // Tangent - RED
	glBindBuffer(GL_ARRAY_BUFFER, nfgData->m_vboID[3]);
	if (triangleShader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(triangleShader->positionAttribute);
		glVertexAttribPointer(triangleShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (const void*)0);
	}
	//------
	if (triangleShader->colorAttribute != -1)
	{
		glUniform4f (triangleShader->colorAttribute, 1.0f, 0.0f, 0.0f, 1.0f);
	}
	glDrawElements(GL_LINES, nfgData->m_numVertice * 2, GL_UNSIGNED_SHORT, indexes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//------------------------------- // Bitangent - GREEN
	glBindBuffer(GL_ARRAY_BUFFER, nfgData->m_vboID[4]);
	if (triangleShader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(triangleShader->positionAttribute);
		glVertexAttribPointer(triangleShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (const void*)0);
	}
	//------
	if (triangleShader->colorAttribute != -1)
	{
		glUniform4f (triangleShader->colorAttribute, 0.0f, 1.0f, 0.0f, 1.0f);
	}
	glDrawElements(GL_LINES, nfgData->m_numVertice * 2, GL_UNSIGNED_SHORT, indexes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//-------------------------------
	SAFE_DEL_ARRAY(indexes)
}
///////////////////////////////////////////////////
void Object::Render()
{
	Shader* shader = ResourceManager::GetInstance()->GetShaderById(m_shaderID);
	glUseProgram(shader->program);
	//-------------- TEXTURE
	if (m_numTexture > 0)
	{
		if (m_numTexture == 1)
		{
			if (shader->m_locTexture1 != -1)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ResourceManager::GetInstance()->GetTextureById(m_texturesID[0])->GetVBOId());
				glUniform1i(shader->m_locTexture1, 0);
			}
		}
		else
		{
			int i = 0;
			int uniformLocation[4] = { shader->m_locBlendMap, shader->m_locTexture1, shader->m_locTexture2, shader->m_locTexture3 };
			for (int k = 0; k < m_numTexture; k++)	// Max m_numTexture is 4, if larger, crashed.
			{
				if (uniformLocation[k] != -1)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, ResourceManager::GetInstance()->GetTextureById(m_texturesID[k])->GetVBOId());
					glUniform1i(uniformLocation[k], i);
					i++;
				}
			}
		}
	}
	//-------------- CUBE
	if (shader->m_cubeUniform != -1)
	{
		if (m_numCubeTexture > 0)
		{
			glActiveTexture(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, ResourceManager::GetInstance()->GetCubeTextureById(m_cubeTexturesID[0])->GetVBOId());
			glUniform1i(shader->m_cubeUniform, 0);
		}
	}
	//--------------------------
	Matrix maModel;
	Camera* activeCamera = SceneManager::GetInstance()->GetActiveCamera();
	if (shader->mvpAttribute != -1)
	{
		{
			Matrix maPos, maRot, maScale;
			maPos.SetTranslation(m_pos);
			maRot.SetRotationXYZ(m_rotate.x, m_rotate.y, m_rotate.z);
			maScale.SetScale(m_scale);
			/*if (m_id == 1)
			{
				static GLfloat rAlpha = 0.035f;
				rAlpha += 0.035;
				if (rAlpha >= 2 * PI)
					rAlpha = 0.0f;
				maModelRot.SetRotationXYZ(rAlpha, rAlpha, rAlpha);
			}*/
			maModel = maScale * maRot * maPos;
		}
		Matrix maMVPMatrix = maModel * SceneManager::GetInstance()->GetActiveCamera()->GetViewProjectMatrix();
		glUniformMatrix4fv (shader->mvpAttribute, 1, GL_FALSE, (const GLfloat*)maMVPMatrix.m);
	}
	//--------------------------
	Model* nfgData = ResourceManager::GetInstance()->GetModelById(m_modelID);
	glBindBuffer(GL_ARRAY_BUFFER, nfgData->m_vboID[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nfgData->m_vboID[1]);
	if (shader->positionAttribute != -1)
	{
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		//glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nfgData->vertices);
		glEnableVertexAttribArray(shader->positionAttribute);
	}
	if (shader->uiTextureAttribute != -1)
	{	
		glVertexAttribPointer(shader->uiTextureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(Vector3)));
		//glVertexAttribPointer(shader->uiTextureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)((char*)nfgData->vertices + sizeof(Vector3)));
		glEnableVertexAttribArray(shader->uiTextureAttribute);
	}
	// Camera
	if (shader->m_cameraPos != -1)
	{
		glUniform3f(shader->m_cameraPos, activeCamera->m_position.x, activeCamera->m_position.y, activeCamera->m_position.z);
	}
	if (shader->m_maModel != -1)
	{
		glUniformMatrix4fv(shader->m_maModel, 1, GL_FALSE, (GLfloat*)maModel.m);
	}
	// Fog
	if (shader->m_fogStart != -1)
	{
		glUniform1f(shader->m_fogStart, SceneManager::GetInstance()->GetFogStart());
	}
	if (shader->m_fogLength != -1)
	{
		glUniform1f(shader->m_fogLength, SceneManager::GetInstance()->GetFogLength());
	}
	// Fire
	static float time = 0.0f;
	time += 0.001f;
	if (shader->m_time != -1)
	{
		glUniform1f(shader->m_time, time);
	}
	// Reflection
	if (shader->m_normL != -1)
	{
		glVertexAttribPointer(shader->m_normL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(Vector3) + sizeof(Vector2)));
		glEnableVertexAttribArray(shader->m_normL);
	}
	if (shader->m_worldMatrix != -1)
	{
		glUniformMatrix4fv(shader->m_worldMatrix, 1, GL_FALSE, (const GLfloat*)&(maModel.m));
	}
	// Direction Lighting
	if (shader->m_lightPosition != -1)
	{
		Object* w1 = SceneManager::GetInstance()->GetObjectByID(2);
		glUniform4f(shader->m_lightPosition, w1->m_pos.x, w1->m_pos.y, w1->m_pos.z, 0.0f);
		//LOGI("AAAA: %d, %.2f, %.2f, %.2f", shader->m_lightPosition, woman1->m_pos.x, woman1->m_pos.y, woman1->m_pos.z);
	}	
	if (shader->m_lightAmbient != -1)
	{
		glUniform4f(shader->m_lightAmbient, 0.1f, 0.1f, 0.1f, 1.0f);	// light of environment: should be black
	}	
	if (shader->m_lightDiffuse != -1)
	{
		glUniform4f(shader->m_lightDiffuse, 1.0f, 1.0f, 1.0f, 1.0f);	// light of sun: should be white
	}
	// Point Lighting
	if (shader->m_maViewModel != -1)
	{
		Matrix maMVMatrix = maModel * SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix();
		glUniformMatrix4fv(shader->m_maViewModel, 1, GL_FALSE, (GLfloat*)maMVMatrix.m);//maModel
	}
	if (shader->m_lightConstantAtn != -1)
	{
		glUniform1f(shader->m_lightConstantAtn, 0.1f);
	}
	if (shader->m_lightLinearAtn != -1)
	{
		glUniform1f(shader->m_lightLinearAtn, 0.00001f);
	}
	if (shader->m_lightQuadraticAtn != -1)
	{
		glUniform1f(shader->m_lightQuadraticAtn, 0.00005f);
	}
	// Point Specular Lighting
	if (shader->m_lightSpecular != -1)
	{
		glUniform4f(shader->m_lightSpecular, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (shader->m_materialShininess != -1)
	{
		glUniform1f(shader->m_materialShininess, 10.0f);
	}
	// Point Fragment Lighting
	if (shader->m_ambientWeight != -1)
	{
		glUniform1f(shader->m_ambientWeight, 0.4f);
	}
	// Normal Mapping
	if (shader->m_tangentL != -1)
	{
		glVertexAttribPointer(shader->m_tangentL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)
																							(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3) + sizeof(Vector3)));
		glEnableVertexAttribArray(shader->m_tangentL);
	}
	if (shader->m_bitangentL != -1)
	{
		glVertexAttribPointer(shader->m_bitangentL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)
																							(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3)));
		glEnableVertexAttribArray(shader->m_bitangentL);
	}
	if (shader->m_textureNormalMap != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ResourceManager::GetInstance()->GetTextureById(39)->GetVBOId());
		glUniform1i(shader->m_textureNormalMap, 1);
	}
	//---------------------------------
	static int count = 0;
	if (count == 0)
	{
		count++;
		//LOGI("AAAA: %d, %d, %d, %d, %.2f, %.2f, %.2f", shader->m_cameraPos, shader->m_modelMatrix, shader->m_fogStart, shader->m_fogLength, activeCamera->m_position.x, activeCamera->m_position.y, activeCamera->m_position.z);
	}
	if (m_shaderID == 11)
	{
		//LOGI("AAAA: %d", shader->m_materialShininess);
	}
	//---------------------------------
	glDrawElements(GL_TRIANGLES, nfgData->m_numIndicate, GL_UNSIGNED_SHORT, (const void*)0);
	//glDrawElements(GL_TRIANGLES, nfgData->m_numIndicate, GL_UNSIGNED_SHORT, nfgData->indicates);
	//-------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////