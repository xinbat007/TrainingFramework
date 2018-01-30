// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shader.h"
#ifdef WIN32
#include <conio.h>
#endif
#include "Globals.h"
#include <math.h>
// thien.pq
//#include "TrainingFramework.h"
#include "Camera.h"
#include "Commons.h"
#include "EffectManager.h"
#include "GS_Welcome.h"
#include "Model.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "StateBase.h"
#include "StatesStack.h"
#include "TouchManager.h"
#include "Utils.h"
///////////////////////////////////////////////////
///////////////////////////////////////////////////
#ifdef WIN32
ESContext* _esCtext;
#endif
Shader *_triangleShader, *_quadShader;
GLuint _vboIdTriangle[2];
GLuint _vboIdQuad[2];
GLuint _vboIdPiece[2];
GLuint _vboIdCoor;
Matrix _maMvpMatrix;
bool   _g_bIsInitCompleted = false;
///////////////////////////////////////////////////
int Init ()
{
	if (_g_bIsInitCompleted)
		return 0;
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );	// Init Background Color
#ifdef WIN32
	glViewport(0, 0, _esCtext->width, _esCtext->height);
#endif
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_STENCIL_TEST);
	//glEnable(GL_SCISSOR_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//---------------------------- Bind Triangle
	Vertex v1[3];
	v1[0].pos.x =  0.0f; v1[0].pos.y =  0.5f; v1[0].pos.z =  0.0f;
	v1[1].pos.x = -0.5f; v1[1].pos.y = -0.5f; v1[1].pos.z =  0.0f;
	v1[2].pos.x =  0.5f; v1[2].pos.y = -0.5f; v1[2].pos.z =  0.0f;
	glGenBuffers(2, _vboIdTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, _vboIdTriangle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v1), v1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unsigned short indexesTriAngle[] = {0, 1, 2};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIdTriangle[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 3, indexesTriAngle, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//---------------------------- Bind Multitexture
	Vertex vQ[4];
	vQ[0].pos.x = -1.0f; vQ[0].pos.y = -1.0f; vQ[0].pos.z =  0.0f; vQ[0].uv.x = 0.0f; vQ[0].uv.y = 1.0f;
	vQ[1].pos.x =  1.0f; vQ[1].pos.y = -1.0f; vQ[1].pos.z =  0.0f; vQ[1].uv.x = 1.0f; vQ[1].uv.y = 1.0f;
	vQ[2].pos.x = -1.0f; vQ[2].pos.y =  1.0f; vQ[2].pos.z =  0.0f; vQ[2].uv.x = 0.0f; vQ[2].uv.y = 0.0f;
	vQ[3].pos.x =  1.0f; vQ[3].pos.y =  1.0f; vQ[3].pos.z =  0.0f; vQ[3].uv.x = 1.0f; vQ[3].uv.y = 0.0f;
	glGenBuffers(2, _vboIdQuad);
	glBindBuffer(GL_ARRAY_BUFFER, _vboIdQuad[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vQ, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unsigned short indexesQuad[] = {0, 1, 2,
									1, 2, 3};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIdQuad[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 6, indexesQuad, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//---------------------------- Bind Coordinate
	Vertex vCoor[6];
	vCoor[0].pos.x = 0.0f;  vCoor[0].pos.y = 0.0f;  vCoor[0].pos.z = 0.0f;
	vCoor[1].pos.x = 1.0f;  vCoor[1].pos.y = 0.0f;  vCoor[1].pos.z = 0.0f;
	vCoor[2].pos.x = 0.0f;  vCoor[2].pos.y = 0.0f;  vCoor[2].pos.z = 0.0f;
	vCoor[3].pos.x = 0.0f;  vCoor[3].pos.y = 1.0f;  vCoor[3].pos.z = 0.0f;
	vCoor[4].pos.x = 0.0f;  vCoor[4].pos.y = 0.0f;  vCoor[4].pos.z = 0.0f;
	vCoor[5].pos.x = 0.0f;  vCoor[5].pos.y = 0.0f;  vCoor[5].pos.z = 1.0f;
	glGenBuffers(1, &_vboIdCoor);
	glBindBuffer(GL_ARRAY_BUFFER, _vboIdCoor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vCoor), vCoor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//----------------------------
	char fileRM[200], fileSM[200], fileEM[200];
#ifdef WIN32
	sprintf(fileRM, "../Resources/RM.txt");
	sprintf(fileSM, "../Resources/SM.txt");
	sprintf(fileEM, "../Resources/EM.txt");
#else
	sprintf(fileRM, "%s/Resources/RM.txt", SD_FOLDER);
	sprintf(fileSM, "%s/Resources/SM.txt", SD_FOLDER);
	sprintf(fileEM, "%s/Resources/EM.txt", SD_FOLDER);
#endif
	ResourceManager::CreateInstance();
	SceneManager::CreateInstance();
	EffectManager::CreateInstance();
	ResourceManager::GetInstance()->ReadRMFile(fileRM);
	SceneManager::GetInstance()->ReadSMFile(fileSM);
	EffectManager::GetInstance()->ReadEMFile(fileEM);
	EffectManager::GetInstance()->Log();
	//----------------------------
	StatesStack::GetInstance()->InitStateStack();
	StateBase* state = new GS_Welcome();// GS_MainMenu();// GS_Welcome();
	StatesStack::GetInstance()->PushState(state);
	//----------------------------
	_triangleShader = ResourceManager::GetInstance()->GetShaderById(0);
	_quadShader  = ResourceManager::GetInstance()->GetShaderById(6);
	_g_bIsInitCompleted = true;
	return 0;
}
void DrawTriangle (GLfloat x, GLfloat y, GLfloat z )
{
	glUseProgram(_triangleShader->program);
	//-----------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, _vboIdTriangle[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIdTriangle[1]);
	if(_triangleShader->positionAttribute != -1)
	{
		glVertexAttribPointer(_triangleShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		glEnableVertexAttribArray(_triangleShader->positionAttribute);
	}
	//-----------------------------------
	static Matrix maModel;
	Matrix tmpModel;
	tmpModel.SetTranslation(x, y, z);
	maModel = maModel * tmpModel;
	_maMvpMatrix = SceneManager::GetInstance()->GetActiveCamera()->CalculateMVPMatrix(maModel);
	glUniformMatrix4fv (_triangleShader->mvpAttribute, 1, GL_FALSE, (GLfloat*)_maMvpMatrix.m);
	glUniform4f(_triangleShader->colorAttribute, 1.0f, 0.0f, 0.0f, 1.0f);
	//-----------------------------------
	glDrawElements(GL_TRIANGLES, 1 * 3, GL_UNSIGNED_SHORT, (const void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void DrawCoordinateSystem()
{
	glUseProgram(_triangleShader->program);
	//-----------------------
	glBindBuffer(GL_ARRAY_BUFFER, _vboIdCoor);
	if(_triangleShader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(_triangleShader->positionAttribute);
		glVertexAttribPointer(_triangleShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
	}
	//-----------------------
	static Matrix maModel;
	static GLfloat rAlpha = 0.0f;
	rAlpha += 0.01f;
	if (rAlpha >= 2 * PI)
		rAlpha = 0.0f;
	//maModel = maModel.SetRotationY(rAlpha);
	//Camera::GetInstance()->SetRotationY(rAlpha);
	_maMvpMatrix = SceneManager::GetInstance()->GetActiveCamera()->CalculateMVPMatrix(maModel);
	glUniformMatrix4fv (_triangleShader->mvpAttribute, 1, GL_FALSE, (GLfloat*)_maMvpMatrix.m);
	//-----------------------
	unsigned short indexes2[] = { 0, 1 };
	glUniform4f (_triangleShader->colorAttribute, 1.0f, 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINES, sizeof(indexes2) / sizeof(unsigned short), GL_UNSIGNED_SHORT, indexes2);
	indexes2[0] = 2; indexes2[1] = 3;
	glUniform4f (_triangleShader->colorAttribute, 0.0f, 1.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINES, sizeof(indexes2) / sizeof(unsigned short), GL_UNSIGNED_SHORT, indexes2);
	indexes2[0] = 4; indexes2[1] = 5;
	glUniform4f (_triangleShader->colorAttribute, 0.0f, 0.0f, 1.0f, 1.0f);
	glDrawElements(GL_LINES, sizeof(indexes2) / sizeof(unsigned short), GL_UNSIGNED_SHORT, indexes2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void DrawQuad(GLuint textID, GLuint vertexBuffer, int x, int y, int width, int height)
{
	glUseProgram(_quadShader->program);
	if (_quadShader->m_locBlendMap != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, textID);
		glUniform1i(_quadShader->m_locBlendMap, 0);
	}
	//--------------------------
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIdQuad[1]);
	if (_quadShader->positionAttribute != -1)
	{
		//glVertexAttribPointer(_quadShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
		glVertexAttribPointer(_quadShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		glEnableVertexAttribArray(_quadShader->positionAttribute);
	}
	if (_quadShader->uiTextureAttribute != -1)
	{
		//glVertexAttribPointer(_quadShader->uiTextureAttribute, 2, GL_FLOAT, GL_FALSE, 0, afTexCoords);
		glVertexAttribPointer(_quadShader->uiTextureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)sizeof(Vector3));
		glEnableVertexAttribArray(_quadShader->uiTextureAttribute);
	}
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_SHORT, (const void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void DrawMultiTextureQuad(int x, int y, int width, int height)
{
	Shader* shader = ResourceManager::GetInstance()->GetShaderById(3);
	glUseProgram(shader->program);
	//----------------
	GLuint textIDBlend = ResourceManager::GetInstance()->GetTextureById(5)->GetVBOId();
	GLuint textID1 = ResourceManager::GetInstance()->GetTextureById(2)->GetVBOId();
	GLuint textID2 = ResourceManager::GetInstance()->GetTextureById(3)->GetVBOId();
	GLuint textID3 = ResourceManager::GetInstance()->GetTextureById(11)->GetVBOId();
	int i = 0;
	if(shader->m_locBlendMap != -1)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textIDBlend);
		glUniform1i(shader->m_locBlendMap, i);
		i++;
	}	
	if(shader->m_locTexture1 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textID1);
		glUniform1i(shader->m_locTexture1, i);
		i++;
	}
	if (shader->m_locTexture2 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textID2);
		glUniform1i(shader->m_locTexture2, i);
		i++;
	}
	if (shader->m_locTexture3 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textID3);
		glUniform1i(shader->m_locTexture3, i);
		i++;
	}
	static int count = 0;
	if (count == 0)
	{
		count++;
		LOGI("AAAA: %d, %d, %d, %d", shader->m_locBlendMap, shader->m_locTexture1, shader->m_locTexture2, shader->m_locTexture3);
	}
	//--------------------------
	Matrix maModel, maModelScale;
	maModelScale.SetScale((GLfloat)width / g_screenWidth, (GLfloat)height / g_screenHeight, 1.0f);
	GLfloat transX = 2.0f * (GLfloat)x / g_screenWidth - 1.0f;
	GLfloat transY = -(2.0f * (GLfloat)y / g_screenHeight - 1.0f);
	maModel.SetTranslation(transX, transY, 0.0f);
	maModel = maModelScale * maModel;
	_maMvpMatrix = SceneManager::GetInstance()->GetActiveCamera()->CalculateMVPMatrix(maModel);
	glUniformMatrix4fv (shader->mvpAttribute, 1, GL_FALSE, (GLfloat*)_maMvpMatrix.m);
	//--------------------------
	glBindBuffer(GL_ARRAY_BUFFER, _vboIdQuad[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIdQuad[1]);
	if (shader->positionAttribute != -1)
	{
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		glEnableVertexAttribArray(shader->positionAttribute);
	}
	if (shader->uiTextureAttribute != -1)
	{
		glVertexAttribPointer(shader->uiTextureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)sizeof(Vector3));
		glEnableVertexAttribArray(shader->uiTextureAttribute);
	}
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_SHORT, (const void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//////////////////////////////////////////////////////////
void DeleteBuffer(GLsizei n, GLuint* buffer)
{
	glDeleteBuffers(n, buffer);
}
//////////////////////////////////////////////////////////
void CleanUp()
{
	glDeleteBuffers(2, _vboIdPiece);
	glDeleteBuffers(1, &_vboIdCoor);
	glDeleteBuffers(2, _vboIdTriangle);
	glDeleteBuffers(2, _vboIdQuad);
	//-------------------------------
    StatesStack::GetInstance()->ClearStateStack();
    StatesStack::GetInstance()->DeleteStatesList();
	//-------------------------------
	StatesStack::DestroyInstance();
	ResourceManager::DestroyInstance();
	EffectManager::DestroyInstance();
	SceneManager::DestroyInstance();
	Utils::DestroyInstance();
	TouchManager::DestroyInstance();
}
//////////////////////////////////////////////////////////
#ifdef WIN32
void Draw (ESContext *esContext)
#else
void Draw ()
#endif
{
	if (!_g_bIsInitCompleted)
		return;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//-------------------
	//DrawTriangle(0.0f, 0.0f, 0.0f);
	//DrawCoordinateSystem();
	StatesStack::GetInstance()->Render();
	//-------------------
#ifdef WIN32
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
#endif
}
//////////////////////////////////////////////////////////
#ifdef WIN32
void Update ( ESContext *esContext, float deltaTime )
#else
void Update ( float deltaTime )
#endif
{
	if (!_g_bIsInitCompleted)
		return;
	StatesStack::GetInstance()->Update(deltaTime);
}
//////////////////////////////////////////////////////////
void Pause()
{
	LOGI("AAAA: TRAINING FRAMEWORK PAUSE");
}
//////////////////////////////////////////////////////////
void Resume()
{
	LOGI("AAAA: TRAINING FRAMEWORK RESUME");
}
//////////////////////////////////////////////////////////
#ifdef WIN32
void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
#else
void Key (unsigned char key, bool bIsPressed)
#endif
{
	StatesStack::GetInstance()->Key(key, bIsPressed);
}
//////////////////////////////////////////////////////////
void MouseFuncDefault(int mouseEvent, int x, int y, int id)
{
	static bool isLeftMouseDown = false;
	static bool isRightMouseDown = false;
#ifdef WIN32
	if (mouseEvent == WM_LBUTTONDOWN)
	{
		isLeftMouseDown = true;
		TouchManager::GetInstance()->OnTouchPress(0, x, y);
	}
	else if (mouseEvent == WM_LBUTTONUP)
	{
		isLeftMouseDown = false;
		TouchManager::GetInstance()->OnTouchRelease(0, x, y);
	}
	else if(mouseEvent == WM_RBUTTONDOWN)
	{
		isRightMouseDown = true;
		TouchManager::GetInstance()->OnTouchPress(1, x, y);
	}
	else if(mouseEvent == WM_RBUTTONUP)
	{
		isRightMouseDown = false;
		TouchManager::GetInstance()->OnTouchRelease(1, x, y);
	}
	else if (mouseEvent == WM_MOUSEMOVE)
	{
		if (isLeftMouseDown)
		{
			TouchManager::GetInstance()->OnTouchMove(0, x, y);
		}
		else if (isRightMouseDown)
		{
			TouchManager::GetInstance()->OnTouchMove(1, x, y);
		}
	}
	else if (mouseEvent == WM_MOUSEWHEEL)
	{
		LOGI("AAAA: mouseEvent: %d, %d", x, y);
	}
	else
	{
		LOGI("AAAA: mouse event: %d", mouseEvent);
	}
#endif
}
//////////////////////////////////////////////////////////
#ifdef WIN32
void ProcessMouseFunc ( ESContext *esContext, unsigned int mouseEvent, int x, int y)
#else
void ProcessMouseFunc ( unsigned int mouseEvent, int x, int y, int id)
#endif
{
#ifdef WIN32
	StatesStack::GetInstance()->Mouse(mouseEvent, x, y, 0);
#else
	StatesStack::GetInstance()->Mouse(mouseEvent, x, y, id);
#endif
}
//////////////////////////////////////////////////////////
void SetWidthHeight(int w, int h)
{
	LOGI("AAAA: SetWidthHeight: %d, %d", w, h);
	g_screenWidth = w;
	g_screenHeight = h;
}
//////////////////////////////////////////////////////////
int _tmain(int argc, char* argv[])
{
#ifdef WIN32
	//#include <vld.h>
	_esCtext = new ESContext();
    esInitContext (_esCtext);
	esCreateWindow (_esCtext, "Hello Triangle",
					g_screenWidth, g_screenHeight,
					ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	if (Init() != 0)
		return 0;
	esRegisterDrawFunc ( _esCtext, Draw );
	esRegisterUpdateFunc ( _esCtext, Update );
	esRegisterKeyFunc ( _esCtext, Key);
	esRegisterMouseFunc  (_esCtext, ProcessMouseFunc);
	esMainLoop ( _esCtext );

	SAFE_DEL(_esCtext);
#endif
	//releasing OpenGL resources
	CleanUp();

#ifdef WIN32
	//identifying memory leaks
	MemoryDump();
	//printf("Press any key...\n");
	_getch();
#endif

	return 0;
}