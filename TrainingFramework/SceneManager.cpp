#include <stdafx.h>
#include "SceneManager.h"
#include "EffectManager.h"
///////////////////////////////////////////////////
SceneManager* SceneManager::ms_pInstance = NULL;
///////////////////////////////////////////////////
SceneManager::SceneManager()
{
	m_MoveMask = 0x0000;
	m_RotateMask = 0x0000;
	m_isRenderNormal = false;
}
///////////////////////////////////////////////////
SceneManager::~SceneManager()
{
	SAFE_DEL_ARRAY(m_Objects);
	SAFE_DEL_ARRAY(m_Cameras);
}
///////////////////////////////////////////////////
void SceneManager::ListObject()
{
	for (int i = 0; i < m_numObject; i++)
	{
		m_Objects[i].Log();
	}
}
///////////////////////////////////////////////////
void SceneManager::ListCamera()
{
	for (int i = 0; i < m_numCamera; i++)
	{
		m_Cameras[i].Log();
	}
}
///////////////////////////////////////////////////
Object* SceneManager::GetObjectByID(int id)
{
	for (int i = 0; i < m_numObject; i++)
	{
		if (m_Objects[i].m_id == id)
			return m_Objects + i;
	}
	return NULL;
}
///////////////////////////////////////////////////
Camera* SceneManager::GetCameraByID(int id)
{
	for (int i = 0; i < m_numCamera; i++)
	{
		if (m_Cameras[i].m_id == id)
			return m_Cameras + i;
	}
	return NULL;
}
///////////////////////////////////////////////////
void SceneManager::Update(float deltaTime)
{
	if (m_MoveMask)
	{
		GetActiveCamera()->Move(m_MoveMask, deltaTime);
	}
	if (m_RotateMask)
	{
		GetActiveCamera()->Rotate(m_RotateMask, deltaTime);
	}
}
///////////////////////////////////////////////////
void SceneManager::Key(unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
	{
		switch(key)
		{
		case 'w':
		case 'W':
			m_MoveMask = 1;
			break;
		case 's':
		case 'S':
			m_MoveMask = 2;
			break;
		case 'a':
		case 'A':
			m_MoveMask = 3;
			break;
		case 'd':
		case 'D':
			m_MoveMask = 4;
			break;
		case 'R':
			m_MoveMask = 5;
			break;
		case 'V':
			m_MoveMask = 6;
			break;
		case 38:	// UP key
			m_RotateMask |= MOVE_FORWARD;
			break;
		case 40:	// DOWN key
			m_RotateMask |= MOVE_BACKWORD;
			break;
		case 37:	// LEFT key
			m_RotateMask |= MOVE_LEFT;
			break;
		case 39:	// RIGHT key
			m_RotateMask |= MOVE_RIGHT;
			break;
		case 'N':
			m_isRenderNormal = !m_isRenderNormal;
			break;
		case 'E':
			if (EffectManager::GetInstance()->IsLoaded())
				EffectManager::GetInstance()->ActiveNextEffect();
			break;
		}
	}
	else
	{
		switch(key)
		{
		case 'w':
		case 'W':
			m_MoveMask = 0;
			break;
		case 's':
		case 'S':
			m_MoveMask = 0;
			break;
		case 'a':
		case 'A':
			m_MoveMask = 0;
			break;
		case 'd':
		case 'D':
			m_MoveMask = 0;
			break;
		case 'R':
			m_MoveMask = 0;
			break;
		case 'V':
			m_MoveMask = 0;
			break;
		case 38:	// UP key
			m_RotateMask &= ~MOVE_FORWARD;
			break;
		case 40:	// DOWN key
			m_RotateMask &= ~MOVE_BACKWORD;
			break;
		case 37:	// LEFT key
			m_RotateMask &= ~MOVE_LEFT;
			break;
		case 39:	// RIGHT key
			m_RotateMask &= ~MOVE_RIGHT;
			break;
		}
	}
}
///////////////////////////////////////////////////
bool SceneManager::ReadSMFile(const char* smDir)
{
	FILE*	pFile;
	char	buff[LINE_LENGTH], myString[LINE_LENGTH];	
	int		myInteger;
	float	x, y, z;
	pFile = fopen (smDir, "r" );
	if (pFile == NULL)
	{
		fputs ("File error", stderr);
		return false;
	}
	//-----------------
	READ_LINE();	// Objects
	sscanf(buff, "#Objects: %u", &m_numObject);
	m_Objects	= new Object[m_numObject];
	for (int i = 0; i < m_numObject; i++)
	{
		READ_LINE();	// comment
		READ_LINE();
		sscanf(buff, "ID %u", &myInteger);
		m_Objects[i].m_id = myInteger;

		READ_LINE();
		sscanf(buff, "MODEL %u", &myInteger);
		m_Objects[i].m_modelID = myInteger;
		
		READ_LINE();
		sscanf(buff, "TEXTURES %u", &myInteger);
		m_Objects[i].m_numTexture = myInteger;
		if (m_Objects[i].m_numTexture > 0)
		{
			m_Objects[i].m_texturesID = new int[m_Objects[i].m_numTexture];
			for (int k = 0; k < m_Objects[i].m_numTexture; k++)
			{
				READ_LINE();
				sscanf(buff, "TEXTURE %u", &myInteger);
				m_Objects[i].m_texturesID[k] = myInteger;
			}
		}
		
		READ_LINE();
		sscanf(buff, "CUBETEXTURES %u", &myInteger);
		m_Objects[i].m_numCubeTexture = myInteger;
		if (m_Objects[i].m_numCubeTexture > 0)
		{
			m_Objects[i].m_cubeTexturesID = new int[m_Objects[i].m_numCubeTexture];
			for (int k = 0; k < m_Objects[i].m_numCubeTexture; k++)
			{
				READ_LINE();
				sscanf(buff, "CUBETEX %u", &myInteger);
				m_Objects[i].m_cubeTexturesID[k] = myInteger;
			}
		}

		READ_LINE();
		sscanf(buff, "SHADER %u", &myInteger);
		m_Objects[i].m_shaderID = myInteger;		
		
		READ_LINE();
		sscanf(buff, "LIGHTS %u", &myInteger);
		m_Objects[i].m_numLight = myInteger;
		if (m_Objects[i].m_numLight > 0)
		{
			m_Objects[i].m_lightsID = new int[m_Objects[i].m_numLight];
			for (int k = 0; k < m_Objects[i].m_numLight; k++)
			{
				READ_LINE();
				sscanf(buff, "LIGHT %u", &myInteger);
				m_Objects[i].m_lightsID[k] = myInteger;
			}
		}

		READ_LINE();
		sscanf(buff, "POSITION %f, %f, %f", &x, &y, &z);
		m_Objects[i].SetPos(x, y, z);
		
		READ_LINE();
		sscanf(buff, "ROTATION %f, %f, %f", &x, &y, &z);
		m_Objects[i].SetRotate(x, y, z);
		
		READ_LINE();
		sscanf(buff, "SCALE %f, %f, %f", &x, &y, &z);
		m_Objects[i].SetScale(x, y, z);
	}

	READ_LINE();
	READ_LINE();	// Cameras
	sscanf(buff, "#Cameras: %u", &m_numCamera);
	m_Cameras	= new Camera[m_numCamera];
	for (int i = 0; i < m_numCamera; i++)
	{
		READ_LINE();
		READ_LINE();
		sscanf(buff, "ID %u", &myInteger);
		m_Cameras[i].m_id = myInteger;

		READ_LINE();
		sscanf(buff, "NEAR %f", &x);
		m_Cameras[i].m_near = x;

		READ_LINE();
		sscanf(buff, "FAR %f", &x);
		m_Cameras[i].m_far = x;

		READ_LINE();
		sscanf(buff, "FOV %f", &x);
		m_Cameras[i].m_fov = x;

		READ_LINE();
		sscanf(buff, "ASPECT %f", &x);
		m_Cameras[i].m_aspect = x;

		READ_LINE();
		sscanf(buff, "MOVING SPEED %u", &myInteger);
		m_Cameras[i].m_movingSpeed = myInteger;

		READ_LINE();
		sscanf(buff, "ROTATING SPEED %u", &myInteger);
		m_Cameras[i].m_rotateSpeed = myInteger;

		READ_LINE();
		sscanf(buff, "UP %f, %f, %f", &x, &y, &z);
		m_Cameras[i].SetUp(x, y, z);

		READ_LINE();
		sscanf(buff, "POSITION %f, %f, %f", &x, &y, &z);
		m_Cameras[i].SetPosition(x, y, z);

		READ_LINE();
		sscanf(buff, "TARGET %f, %f, %f", &x, &y, &z);
		m_Cameras[i].SetTarget(x, y, z);

		m_Cameras[i].Init();
	}

	READ_LINE();
	READ_LINE();
	sscanf(buff, "FOG_START %f", &x);
	m_fogStart = x;
	READ_LINE();
	sscanf(buff, "FOG_LENGTH %f", &x);
	m_fogLength = x;
	//-----------------
	fclose (pFile);
	//-----------------
	if (m_numCamera > 0)
		SetActiveCamera(0);
	return true;
}
///////////////////////////////////////////////////
void SceneManager::Render()
{
	//LOGI("AAAA: render: %d", m_numObject);
	for (int i = 0; i < m_numObject; i++)
	{
		m_Objects[i].Render();
		if (m_isRenderNormal)
		{
			m_Objects[i].RenderNormalTangentBitangent();
		}
	}
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
