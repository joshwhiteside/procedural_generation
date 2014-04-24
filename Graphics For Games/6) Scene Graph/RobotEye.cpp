#include "RobotEye.h"


RobotEye::RobotEye(Shader * shader)
{
	SceneNode::sceneNodeShader = shader;

	lazerMin = 0.0f;
	lazerMax = 1000.0f;


	this->mesh			= CubeRobot::cube;
	this->colour		= Vector4(1,0,1,1);
	modelScale			= Vector3(1,1,1);

	SetTransform(Matrix4::Translation(Vector3(2,2,995)));
	SetModelScale(Vector3(0,2,1000));


	SceneNode::SetChildrenShaderToMine();


}


RobotEye::~RobotEye(void)
{
}

void RobotEye::PreDraw(OGLRenderer &r)
{
	SceneNode::PreDraw(r);

}

void RobotEye::SetSpecificUniforms(OGLRenderer &r)
{
	SceneNode::SetSpecificUniforms(r);

	if(parent != NULL)
	{
		Vector3 parentPosition = parent->GetWorldTransform().GetPositionVector();
		glUniform3fv(glGetUniformLocation(sceneNodeShader->GetProgram(),"cubeManWorldPosition"),1,(float*)&parentPosition);
		glUniform1f(glGetUniformLocation(sceneNodeShader->GetProgram(), "minDistance"), lazerMin);
		glUniform1f(glGetUniformLocation(sceneNodeShader->GetProgram(), "maxDistance"), lazerMax);
	}


}

