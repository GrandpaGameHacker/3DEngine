#include "Utilities/Camera/Camera.h"
#include <SDL2/SDL.h>
#include <gl/glew.h>
#include <glm/gtx/quaternion.hpp>
#include "Utilities/Logger.h"
Camera::Camera()
{
	EMode = CameraMode::Normal;
	EType = CameraType::PerspectiveFreecam;
	EMovement = CameraMove::None;

	ViewportX = 0; ViewportY = 0;
	WindowHeight = 0; WindowWidth = 0;

	Aspect = 0;
	NClip = 0.01;
	FClip = 100;

	Up = glm::vec3(0, 1, 0);
	FOV = 45;
	
	Rotation = glm::quat(1, 0, 0, 0);
	Position = glm::vec3(0, 0, 0);
	PositionDelta = glm::vec3(0, 0, 0);

	Yaw = 0;
	Pitch = 0;
	Roll = 0;

	Scale = 0.1f;
	RotationScale = 0.008f;
	RollScale = 0.001f;

	MaxYawRate = 5;
	MaxPitchRate = 5;
	MaxRollRate = 5;

	bRotating = false;
	bMoving = false;
	bRolling = false;
	
	LookAt = glm::vec3(0, 0, 0);
	Direction = glm::vec3(0, 0, 0);
	Mouse = glm::vec3(0, 0, 0);

	Projection = glm::mat4(1);
	View = glm::mat4(1);
	Model = glm::mat4(1);
	MVP = glm::mat4(1);

	Logger::LogInfo("Camera::Camera()", "Initialized Default Camera");
}

void Camera::Update()
{

	Projection = glm::mat4(1);
	View = glm::mat4(1);
	Model = glm::mat4(1);
	MVP = glm::mat4(1);

	if (bMoving) {
		Move(EMovement);
	}

	switch (EMode)
	{
	case CameraMode::Vertex:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case CameraMode::Wireframe:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case CameraMode::Normal:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	Direction = glm::normalize(LookAt - Position);
	if(EType == CameraType::Orthographic)
	{
		Projection = glm::ortho(-1.5f * (float)Aspect, -1.5f * (float)Aspect, -1.5f, -1.5f, -10.f, -10.f);
	}
	else if(EType == CameraType::PerspectiveFreecam)
	{
		Projection = glm::perspective(FOV, Aspect, NClip, FClip);
		glm::vec3 PAxis = glm::cross(Direction, Up);
		glm::quat QPitch = glm::angleAxis(Pitch, PAxis);
		glm::quat QYaw = glm::angleAxis(Yaw, Up);
		glm::quat QRoll = glm::angleAxis(Roll, Direction);
		glm::quat QTemp = glm::cross(QPitch, QYaw);
		QRoll = glm::normalize(QRoll);
		QTemp = glm::normalize(QTemp);
		Direction = glm::rotate(QTemp, Direction);
		Up = glm::rotate(QRoll, Up);
		Position += PositionDelta;
		LookAt = Position + Direction * 1.0f;

		Yaw *= 0.5f;
		Pitch *= 0.5f;
		Roll *= 0.5f;
		PositionDelta *= 0.1f;
	}

	View = glm::lookAt(Position, LookAt, Up);
	MVP = Projection * View * Model;
}

void Camera::Reset()
{
	Up = glm::vec3(0, 1, 0);
}

void Camera::SetMovement(CameraMove move, bool moving)
{
	EMovement = move;
	bMoving = moving;
}

void Camera::Move(CameraMove direction)
{
	if(EType == CameraType::PerspectiveFreecam)
	{
		switch(direction)
		{
		case CameraMove::Up:
			PositionDelta += Up * Scale;
			break;
		case CameraMove::Down:
			PositionDelta -= Up * Scale;
			break;
		case CameraMove::Left:
			PositionDelta -= glm::cross(Direction, Up) * Scale;
			break;
		case CameraMove::Right:
			PositionDelta += glm::cross(Direction, Up) * Scale;
			break;
		case CameraMove::Forward:
			PositionDelta += Direction * Scale;
			break;
		case CameraMove::Back:
			PositionDelta -= Direction * Scale;
			break;
		case CameraMove::None:
			return;
		}
	}
}

void Camera::ChangePitch(float degrees)
{
	if (degrees < -MaxPitchRate) {
		degrees = -MaxPitchRate;
	}
	else if (degrees > MaxPitchRate) {
		degrees = MaxPitchRate;
	}
	Pitch += degrees;

	if (Pitch > 360.0f) {
		Pitch -= 360.0f;
	}
	else if (Pitch < -360.0f) {
		Pitch += 360.0f;
	}
}

void Camera::ChangeYaw(float degrees)
{
	if (degrees < -MaxYawRate) {
		degrees = -MaxYawRate;
	}
	else if (degrees > MaxYawRate) {
		degrees = MaxYawRate;
	}

	if (Pitch > 90 && Pitch < 270 || (Pitch < -90 && Pitch > -270)) {
		Yaw -= degrees;
	}
	else {
		Yaw += degrees;
	}

	if (Yaw > 360.0f) {
		Yaw -= 360.0f;
	}
	else if (Yaw < -360.0f) {
		Yaw += 360.0f;
	}
}

void Camera::ChangeRoll(float degrees)
{
	Roll += degrees;
}

void Camera::Rotate2D(int x, int y)
{
	glm::vec3 MouseDelta = Mouse - glm::vec3(x, y, 0);
	if(bRotating)
	{
		ChangeYaw(RotationScale * MouseDelta.x);
		ChangePitch(RotationScale * MouseDelta.y);
	}
	Mouse = glm::vec3(x, y, 0);
}

void Camera::Roll2D(float y)
{
	ChangeRoll(glm::radians(y*2));
}

void Camera::SetType(CameraType type)
{
	EType = type;
}

void Camera::SetMode(CameraMode mode)
{
	EMode = mode;
	Up = glm::vec3{ 0,1,0 };
	Rotation = glm::quat(1, 0, 0, 0);
}

void Camera::SetPosition(glm::vec3 pos)
{
	Position = pos;
}

void Camera::SetLookAtTarget(glm::vec3 pos)
{
	LookAt = pos;
}

void Camera::SetFOV(double fov)
{
	FOV = fov;
}

void Camera::SetViewport(int loc_x, int loc_y, int width, int height)
{
	ViewportX = loc_x;
	ViewportY = loc_y;
	WindowWidth = width;
	WindowHeight = height;
	Aspect = static_cast<double>(width) / static_cast<double>(height);
}

void Camera::SetClipping(double nearClip, double farClip)
{
	NClip = nearClip;
	FClip = farClip;
}

void Camera::SetPos(int button, int state, int x, int y)
{
	if (button == SDL_BUTTON_LEFT && state == 1)
	{
		bRotating = true;
	}
	else if (button == SDL_BUTTON_LEFT && state != 1)
	{
		bRotating = false;
	}

	Mouse = glm::vec3(x, y, 0);
}

CameraType Camera::GetType()
{
	return EType;
}

CameraMode Camera::GetMode()
{
	return EMode;
}

void Camera::GetViewport(int& loc_x, int& loc_y, int& width, int& height)
{
	loc_x = ViewportX;
	loc_y = ViewportY;
	width = WindowWidth;
	height = WindowHeight;
}

void Camera::GetMVP(glm::mat4& mvp)
{
	mvp = MVP;
}
