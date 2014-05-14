#include "Camera.h"

void Camera::UpdateCamera(float msec)	{
	float speed = msec;

	pitch -=(Window::GetMouse()->GetRelativePosition().y);
	yaw	  -=(Window::GetMouse()->GetRelativePosition().x);

	pitch = min(pitch,90.f);
	pitch = max(pitch,-90.0f);

	if(!Window::GetKeyboard()->KeyDown(KEYBOARD_Q)) speed *= 0.1f;

	if(yaw<0)	{
		yaw+=360.0f;
	}

	if(yaw>360.0f)	{
		yaw-=360.0f;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_W))	{
		position += Matrix4::Rotation(yaw,Vector3(0.0f,1.0f,0.0f)) *
					Vector3(0.0f,0.0f,-1.0f) * speed;
		//position.y += pitch;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_S))	{
		position -= Matrix4::Rotation(yaw,Vector3(0.0f,1.0f,0.0)) *
					Vector3(0.0f,0.0f,-1.0f) * speed;
	}


	if(Window::GetKeyboard()->KeyDown(KEYBOARD_A))	{
		position += Matrix4::Rotation(yaw,Vector3(0.0f,1.0f,0.0f)) *
					Vector3(-1.0f,0.0f,0.0f) * speed;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_D))	{
		position -= Matrix4::Rotation(yaw,Vector3(0.0f,1.0f,0.0)) *
					Vector3(-1.0f,0.0f,0.0f) * speed;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_Z))	{
		roll+=1.0f;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_X))	{
		roll-=1.0f;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT))	{
		position.y +=speed;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE))	{
		position.y -=speed;
	}

}


Matrix4 Camera::BuildViewMatrix()	{
	return Matrix4::Rotation(-pitch,Vector3(1,0,0)) *
		   Matrix4::Rotation(-yaw,	Vector3(0,1,0)) * 
		   //Matrix4::Rotation(-roll,	Vector3(0,0,1)) * 
		   Matrix4::Translation(-position);
}