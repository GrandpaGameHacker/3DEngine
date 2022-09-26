#include "Camera.h"
#include <SDL2/SDL.h>
#include <gl/glew.h>
#include <glm/gtx/quaternion.hpp>
Camera::Camera()
{
	EMode = CameraMode::Normal;
	EType = CameraType::Perspective;

	ViewportX = 0; ViewportY = 0;
	WindowHeight = 0; WindowWidth = 0;
	Aspect = 0;
	NClip = 0.01;
	FClip = 100;

	Yaw = 0;
	Pitch = 0;
	Roll = 0;

	Up = glm::vec3(0, 1, 0);
	FOV = 45;
	Rotation = glm::quat(1, 0, 0, 0);
	Position = glm::vec3(0, 0, 0);
	PositionDelta = glm::vec3(0, 0, 0);

	Scale = 0.1f;
	RotationScale = 0.08;
	MaxYawRate = 5;
	MaxPitchRate = 5;
	MaxRollRate = 5;
	bMoving = false;

	LookAt = glm::vec3(0, 0, 0);
	Direction = glm::vec3(0, 0, 0);
	Mouse = glm::vec3(0, 0, 0);

		Projection = glm::mat4(1);
	View = glm::mat4(1);
	Model = glm::mat4(1);
	MVP = glm::mat4(1);
}

void Camera::Update()
{
	Direction = glm::normalize(LookAt - Position);
	glViewport(ViewportX, ViewportY, WindowWidth, WindowHeight);

	if(EType == CameraType::Orthographic)
	{
		Projection = glm::ortho(-1.5f * (float)Aspect, -1.5f * (float)Aspect, -1.5f, -1.5f, -10.f, -10.f);
	}
	else if(EType == CameraType::Perspective)
	{
		Projection = glm::perspective(FOV, Aspect, NClip, FClip);
		glm::vec3 Axis = glm::cross(Direction, Up);
		glm::quat QPitch = glm::angleAxis(Pitch, Axis);
		glm::quat QYaw = glm::angleAxis(Yaw, Up);
		glm::quat QTemp = glm::cross(QPitch, QYaw);
		QTemp = glm::normalize(QTemp);
		Direction = glm::rotate(QTemp, Direction);
		Position += PositionDelta;
		LookAt = Position + Direction * 1.0f;

		Yaw *= 0.5f;
		Pitch *= 0.5f;
		Roll *= 0.5f;
		PositionDelta *= 0.1f;
	}

	View = glm::lookAt(Position, LookAt, Up);
	Model = glm::mat4(1);
	MVP = Projection * View * Model;
}

void Camera::Reset()
{
	Up = glm::vec3(0, 1, 0);
}

void Camera::Move(CameraMove direction)
{
	if(EType == CameraType::Perspective)
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
	if (degrees < -MaxRollRate)
	{
		degrees = -MaxRollRate;
	}
	else if (degrees < MaxRollRate)
	{
		degrees = MaxRollRate;
	}
	Roll += degrees;

	if (Roll > 360.f)
	{
		Roll -= 360.f;
	}
	else if (Roll < -360.f)
	{
		Roll += 360.f;
	}

}

void Camera::Rotate2D(int x, int y)
{
	glm::vec3 MouseDelta = Mouse - glm::vec3(x, y, 0);
	if(bMoving)
	{
		ChangeYaw(RotationScale * MouseDelta.x);
		ChangePitch(RotationScale * MouseDelta.y);
	}
	Mouse = glm::vec3(x, y, 0);
}

void Camera::SetType(CameraType type)
{
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

void Camera::SetClipping(double near, double far)
{
	NClip = near;
	FClip = far;
}

void Camera::SetPos(int button, int state, int x, int y)
{
	if (button == SDL_BUTTON_LEFT && state == 1)
	{
		bMoving = true;
	}
	else if (button == SDL_BUTTON_LEFT && state != 1)
	{
		bMoving = false;
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
