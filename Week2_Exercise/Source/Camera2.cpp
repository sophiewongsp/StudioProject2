#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera2::Update(double dt)
{
	static const float CAMERA_SPEED = 100.f;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	target = position + view;
	if (target.z >-490.f && target.z < 490.f && target.x > -200.f && target.x < 800.f && target.y > -70.f && target.y < 900.f)
	{
		if (Application::IsKeyPressed('A'))
		{
			position = position - right;
			target = position + view;
		}
		else if (Application::IsKeyPressed('D'))
		{
			position = position + right;
			target = position + view;
		}
		else if (Application::IsKeyPressed('W'))
		{
			position = position + view.Normalized();
			target = position + view;
		}
		else if (Application::IsKeyPressed('S'))
		{
			position = position - view.Normalized();
			target = position + view;
		}
		else if (Application::IsKeyPressed('Q'))
		{
			position = position + up;
			target = position + view;
		}
		else if (Application::IsKeyPressed('E'))
		{
			position = position - up;
			target = position + view;
		}
	}
	else
	{
		if (target.x >= 500.f)
			position.x = position.x - 1;
		else
			position.x = position.x + 1;

		if (target.y >= 900.f)
		{
			position.y = position.y - 1;
		}
		else
		{
			position.y = position.y + 1;
		}
		if (target.z >= 480.f)
			position.z = position.z - 1;
		else
			position.z = position.z + 1;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;
	}
	else if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;;
	}
	else if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);

		right = rotation * right;
		view = rotation * view;
		target = position + view;
	}
	else if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);

		right = rotation * right;
		view = rotation * view;
		target = position + view;
	}
	else if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}