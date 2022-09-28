#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraType
{
	Orthographic,
	PerspectiveFreecam,
};

enum class CameraMode
{
	Vertex,
	Wireframe,
	Normal,
};

enum class CameraMove
{
	None,
	Up, Down, Left, Right, Forward, Back

};

class Camera
{
public:
	~Camera() = default;
	Camera();

	void Update();
	void Reset();
	void Move(CameraMove direction);

	void ChangePitch(float degrees);
	void ChangeYaw(float degrees);
	void ChangeRoll(float degrees);
	void Rotate2D(int x, int y); // for mouse input
	void Roll2D(float y);

	void SetType(CameraType type);
	void SetMode(CameraMode mode);
	void SetMovement(CameraMove move, bool moving);
	void SetPosition(glm::vec3 pos);
	void SetLookAtTarget(glm::vec3 pos);
	void SetFOV(double fov);
	void SetViewport(int loc_x, int loc_y, int width, int height);
	void SetClipping(double near, double farClip);
	void SetPos(int button, int state, int x, int y); //mouse_input

	CameraType GetType();
	CameraMode GetMode();
	void GetViewport(int &loc_X, int& loc_y, int& width, int& height);
	void GetMVP(glm::mat4& mvp);
private:
	CameraType EType;
	CameraMode EMode;
	CameraMove EMovement;
	int ViewportX, ViewportY;
	int WindowWidth, WindowHeight;

	double Aspect;
	double FOV;
	double NClip;
	double FClip;
	bool bRotating;
	bool bMoving;
	bool bRolling;
	float Scale;
	float RotationScale;
	float RollScale;
	float Yaw, Pitch, Roll;

	float MaxYawRate, MaxPitchRate, MaxRollRate;

	glm::vec3 Position;
	glm::vec3 PositionDelta;
	glm::vec3 LookAt;
	glm::vec3 Direction;

	glm::vec3 Up;
	glm::quat Rotation;
	glm::vec3 Mouse;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 MVP;
};

