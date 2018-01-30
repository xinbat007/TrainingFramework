#pragma once

//#include "Matrix.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

const unsigned int MOVE_FORWARD		= 0x0001;
const unsigned int MOVE_BACKWORD	= 0x0010;
const unsigned int MOVE_LEFT		= 0x0100;
const unsigned int MOVE_RIGHT		= 0x1000;

class Camera
{
private:
	Matrix			maProjection, maView, maWorld;
	Matrix			maMvpMatrix;
	void			CalculateViewMatrix();
	Vector3			xAxis, yAxis, zAxis;
	bool			m_isNeedUpdateCamera;
public:
	int				m_id, m_movingSpeed, m_rotateSpeed;
	GLfloat			m_near, m_far, m_fov, m_aspect;
	Vector3			m_position, m_target, m_up;
	void			Init();
	Matrix&			GetViewProjectMatrix();
	Matrix&			GetViewMatrix();
	Matrix&			CalculateMVPMatrix(Matrix maModel);
	void			Move(unsigned int moveMask, float deltaTime);
	void			Rotate(unsigned int rotateMask, float deltaTime);
	void			Log();
	GLfloat			GetNearPlane()								 { return m_near; }
	GLfloat			GetFarPlane()								 { return m_far; }
	void			SetUp(GLfloat x, GLfloat y, GLfloat z)		 { m_up.x       = x; m_up.y       = y; m_up.z       = z; }
	void			SetPosition(GLfloat x, GLfloat y, GLfloat z) { m_position.x = x; m_position.y = y; m_position.z = z; }
	void			SetTarget(GLfloat x, GLfloat y, GLfloat z)	 { m_target.x   = x; m_target.y   = y; m_target.z   = z; }
};