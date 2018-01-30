#include <stdafx.h>
#include "Camera.h"
#include "Commons.h"
#include "Globals.h"
////////////////////////////////////////////////////////////////////////
const double LIMIT_VIEW_ANGLE = cos(PI/2*0.2);
////////////////////////////////////////////////////////////////////////
void Camera::Init()
{
	maProjection.SetPerspective(PI / 4, g_screenWidth > g_screenHeight ?
								g_screenWidth / g_screenHeight : g_screenHeight / g_screenWidth,
								m_near, m_far);
	m_isNeedUpdateCamera = true;
	CalculateViewMatrix();
}
////////////////////////////////////////////////////////////////////////
Matrix&	Camera::CalculateMVPMatrix(Matrix maModel)
{
#ifdef WIN32
	maMvpMatrix = maModel * maView * maProjection;
#else
	maMvpMatrix = (Matrix) (maModel * maView * maProjection);
#endif
	return maMvpMatrix;
}
////////////////////////////////////////////////////////////////////////
Matrix& Camera::GetViewProjectMatrix()
{
	CalculateViewMatrix();
	return maView * maProjection;
}
////////////////////////////////////////////////////////////////////////
Matrix& Camera::GetViewMatrix()
{
	CalculateViewMatrix();
	return maView;;
}
////////////////////////////////////////////////////////////////////////
void Camera::CalculateViewMatrix()
{
	if (m_isNeedUpdateCamera)
	{
		m_isNeedUpdateCamera = false;
		zAxis = (m_position - m_target).Normalize();
		xAxis = (m_up.Cross(zAxis)).Normalize();
		yAxis = (zAxis.Cross(xAxis)).Normalize();
		maView.m[0][0] = xAxis.x; maView.m[0][1] = yAxis.x; maView.m[0][2] = zAxis.x; maView.m[0][3] = 0.0f;
		maView.m[1][0] = xAxis.y; maView.m[1][1] = yAxis.y; maView.m[1][2] = zAxis.y; maView.m[1][3] = 0.0f;
		maView.m[2][0] = xAxis.z; maView.m[2][1] = yAxis.z; maView.m[2][2] = zAxis.z; maView.m[2][3] = 0.0f;
		maView.m[3][0] = -m_position.Dot(xAxis); maView.m[3][1] = -m_position.Dot(yAxis);
		maView.m[3][2] = -m_position.Dot(zAxis); maView.m[3][3] = 1.0f;

		maWorld.m[0][0] = xAxis.x; maWorld.m[0][1] = xAxis.y; maWorld.m[0][2] = xAxis.z; maWorld.m[0][3] = 0.0f;
		maWorld.m[1][0] = yAxis.x; maWorld.m[1][1] = yAxis.y; maWorld.m[1][2] = yAxis.z; maWorld.m[1][3] = 0.0f;
		maWorld.m[2][0] = zAxis.x; maWorld.m[2][1] = zAxis.y; maWorld.m[2][2] = zAxis.z; maWorld.m[2][3] = 0.0f;
		maWorld.m[3][0] = m_position.x;	maWorld.m[3][1] = m_position.y;
		maWorld.m[3][2] = m_position.z; maWorld.m[3][3] = 1.0f;
	}
}
////////////////////////////////////////////////////////////////////////
void Camera::Move(unsigned int moveMask, float deltaTime)
{
	if (!moveMask)
	{
		return;
	}
	float time = 0.5f;
	float speed = 1.0;
	m_isNeedUpdateCamera = true;
	Vector3 deltaMove = -(m_position - m_target).Normalize();
	if (moveMask == 1)
	{
		deltaMove = deltaMove * time * speed;
		m_position += deltaMove;
		m_target += deltaMove;
	}
	else if (moveMask == 2)
	{
		deltaMove = deltaMove * time * speed;
		m_position -= deltaMove;
		m_target -= deltaMove;
	}
	else if (moveMask == 3)
	{
		deltaMove = m_up.Cross(deltaMove).Normalize() * time * speed;
		m_position += deltaMove;
		m_target += deltaMove;
	}
	else if (moveMask == 4)
	{
		deltaMove = m_up.Cross(deltaMove).Normalize() * time * speed;
		m_position -= deltaMove;
		m_target -= deltaMove;
	}
	else if (moveMask == 5)
	{
		deltaMove = m_up.Modulate(deltaMove).Normalize() * time * speed;
		m_position -= deltaMove;
		m_target -= deltaMove;
	}
	else if (moveMask == 6)
	{
		deltaMove = m_up.Modulate(deltaMove).Normalize() * time * speed;
		m_position += deltaMove;
		m_target += deltaMove;
	}
	CalculateViewMatrix();
}
////////////////////////////////////////////////////////////////////////
void Camera::Rotate(unsigned int rotateMask, float deltaTime)
{
	if (!rotateMask)
		return;
	float time = 0.05f;
	float speed = 1.0;

	/*Vector3 position = m_pPosition;
	if (rotateMask & MOVE_FORWARD)
	{
		m_pPosition.y = m_pTarget.y;
		CalculateViewMatrix();
	}*/
	m_isNeedUpdateCamera = true;
	Vector3 position = m_position;
	if (rotateMask & MOVE_FORWARD || rotateMask & MOVE_BACKWORD)
	{
		m_position.y = m_target.y;
		CalculateViewMatrix();
	}
	//----------------------------------------
	Matrix maRotate;
	if (rotateMask & MOVE_FORWARD)
	{
		maRotate.SetRotationX(0.01f);
	}
	else if (rotateMask & MOVE_BACKWORD)
	{
		maRotate.SetRotationX(-0.01f);
	}
	else if (rotateMask & MOVE_LEFT)
	{
		maRotate.SetRotationY(0.01f);
	}
	else if (rotateMask & MOVE_RIGHT)
	{
		maRotate.SetRotationY(-0.01f);
	}
	//----------------------------------------
	//Vector4 localTarget(0.0f, 0.0f, (-m_position + m_target).Length(), 1.0f);
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(m_position - m_target).Length(), 1.0f);
	localTarget = localTarget * maRotate;
	localTarget = localTarget * maWorld;
	Vector3 target = Vector3(localTarget.x, localTarget.y, localTarget.z);
	m_position = position;
	//Limit the angle
	if (abs((target - m_position).Normalize().Dot(m_up.Normalize())) > LIMIT_VIEW_ANGLE )
		return;
	m_target = target;
	//PushNewTarget();
	m_isNeedUpdateCamera = true;
	CalculateViewMatrix();
}
////////////////////////////////////////////////////////////////////////
void Camera::Log()
{
	LOGI("Camera: %d, %f, %f, %f, %f, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f",
					m_id, m_near, m_far, m_fov, m_aspect, m_movingSpeed, m_rotateSpeed,
					m_up.x, m_up.y, m_up.z,
					m_position.x, m_position.y, m_position.z,
					m_target.x, m_target.y, m_target.z
					);
}
////////////////////////////////////////////////////////////////////////