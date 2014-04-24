#include "Renderer.h"

Renderer::Renderer(Window &parent)	: OGLRenderer(parent)	{
	CubeRobot::CreateCube();
	camera		= new Camera();

	currentShader	= new Shader(SHADERDIR"SceneVertex.glsl",
									SHADERDIR"SceneFragment.glsl");

	eyeShader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragmentLAZER.glsl");

	if(!currentShader->LinkProgram() || !eyeShader->LinkProgram())	{
		return;
	}

	projMatrix = Matrix4::Perspective(1.0f,10000.0f,
							(float)width/(float)height,45.0f);
	camera->SetPosition(Vector3(0,30,175));

	root = new SceneNode();
	CubeRobot * cubeRobot = new CubeRobot(currentShader);
	cubeRobot->GetHead()->AddChild(new RobotEye(eyeShader));
	root->AddChild(cubeRobot);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	init = true;
}

Renderer::~Renderer(void)	{
	delete root;
	CubeRobot::DeleteCube();
}

void Renderer::UpdateScene(float msec)	{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer::RenderScene(float msec)	{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex") , 1);

	DrawNode(root);
	glUseProgram(0);

	SwapBuffers();
}

void Renderer::DrawNode(SceneNode*n)	{
	
	if(n->GetMesh())	{

		//Matrix4 transform = n->GetWorldTransform() * 
		//					Matrix4::Scale(n->GetModelScale());
		//glUniformMatrix4fv(
		//	glGetUniformLocation(currentShader->GetProgram(),
		//	"modelMatrix"), 1, false, (float*)&transform);

		//glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
		//	"nodeColour"),1,(float*)&n->GetColour());

		//glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		//	"useTexture"),(int)n->GetMesh()->GetTexture());
		////n->Draw

		//n->PreDraw(*this);
		n->Draw(*this);
	}

	for(vector<SceneNode*>::const_iterator		i = n->GetChildIteratorStart();		i!= n->GetChildIteratorEnd(); ++i) {
		
		if((*i)->GetMesh())
		{
			(*i)->PreDraw(*this);
		}
		
		
		DrawNode(*i);
	}
}