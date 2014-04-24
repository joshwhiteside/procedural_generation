#include "CubeRobot.h"
#include <math.h>
Mesh* CubeRobot::cube = NULL;

CubeRobot::CubeRobot(Shader * shader )	{
	
	SceneNode::sceneNodeShader = shader;

	SceneNode*body = new SceneNode(cube,Vector4(1,0,0,1));
	body->SetModelScale(Vector3(10,15,5));
	AddChild(body);

	head = new SceneNode(cube,Vector4(0,1,0,1));
	head->SetModelScale(Vector3(5,5,5));
	head->SetTransform(Matrix4::Translation(Vector3(0,30,0)));
	body->AddChild(head);

	rightEye = new SceneNode(cube,Vector4(1,0,0,1));
	rightEye->SetModelScale(Vector3(2,2,2));
	rightEye->SetTransform(Matrix4::Translation(Vector3(2,2,5)));
	head->AddChild(rightEye);
	

	leftArm = new SceneNode(cube,Vector4(0,0,1,1));
	leftArm->SetModelScale(Vector3(3,-18,3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12,30,-1)));
	body->AddChild(leftArm);

	rightArm = new SceneNode(cube,Vector4(0,0,1,1));
	rightArm->SetModelScale(Vector3(3,-18,3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12,30,1)));
	body->AddChild(rightArm);

	SceneNode* leftLeg = new SceneNode(cube,Vector4(0,0,1,1));
	leftLeg->SetModelScale(Vector3(3,-17.5,3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8,0,0)));
	body->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(cube,Vector4(0,0,1,1));
	rightLeg->SetModelScale(Vector3(3,-17.5,3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8,0,0)));
	body->AddChild(rightLeg);
	
	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);

	leftLeg->SetBoundingRadius(18.0f);
	rightLeg->SetBoundingRadius(18.0f);

	angle = 0.0f;
}

CubeRobot::CubeRobot()	{
	SceneNode*body = new SceneNode(cube,Vector4(1,0,0,1));
	body->SetModelScale(Vector3(10,15,5));
	AddChild(body);

	head = new SceneNode(cube,Vector4(0,1,0,1));
	head->SetModelScale(Vector3(5,5,5));
	head->SetTransform(Matrix4::Translation(Vector3(0,30,0)));
	body->AddChild(head);

	rightEye = new SceneNode(cube,Vector4(1,0,0,1));
	rightEye->SetModelScale(Vector3(2,2,2));
	rightEye->SetTransform(Matrix4::Translation(Vector3(2,2,5)));
	head->AddChild(rightEye);
	

	leftArm = new SceneNode(cube,Vector4(0,0,1,1));
	leftArm->SetModelScale(Vector3(3,-18,3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12,30,-1)));
	body->AddChild(leftArm);

	rightArm = new SceneNode(cube,Vector4(0,0,1,1));
	rightArm->SetModelScale(Vector3(3,-18,3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12,30,1)));
	body->AddChild(rightArm);

	SceneNode* leftLeg = new SceneNode(cube,Vector4(0,0,1,1));
	leftLeg->SetModelScale(Vector3(3,-17.5,3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8,0,0)));
	body->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(cube,Vector4(0,0,1,1));
	rightLeg->SetModelScale(Vector3(3,-17.5,3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8,0,0)));
	body->AddChild(rightLeg);
	
	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);

	leftLeg->SetBoundingRadius(18.0f);
	rightLeg->SetBoundingRadius(18.0f);

	angle = 0.0f;

}


void CubeRobot::Update(float msec)	{
	
	angle += msec * 0.01f;

	//transform = transform *Matrix4::Rotation(msec / 10.0f, Vector3(0,1,0));

	//head->SetTransform(head->GetTransform() * 	Matrix4::Rotation(-(cos(msec) * 0.5f) + 1, Vector3(1,0,0)));

	float value = (cos(angle) * 5.5f) + 1.0;
	
	/*
	head->SetTransform(Matrix4::Translation(Vector3(0,30,0)) * Matrix4::Rotation(value, Vector3(1,0,0)));

	leftArm->SetTransform(leftArm->GetTransform() * 
		Matrix4::Rotation(-msec / 10.0f, Vector3(1,0,0)));

	rightArm->SetTransform(rightArm->GetTransform() *
		Matrix4::Rotation(msec / 10.0f, Vector3(1,0,0)));
		*/
	SceneNode::Update(msec);
}

void CubeRobot::Draw(OGLRenderer &r){
	SceneNode::Draw(r);
}