#include "SceneNode.h"
//#include "../../nclgl/SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour)	{
	this->mesh			= mesh;
	this->colour		= colour;
	parent				= NULL;
	modelScale			= Vector3(1,1,1);
	sceneNodeShader     = NULL;

	boundingRadius		= 1.0f;
	distanceFromCamera  = 0.0f;
}

SceneNode::~SceneNode(void)	{
	for(unsigned int i = 0; i < children.size(); ++i)	{
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s)	{
	children.push_back(s);
	s->parent = this;
}

void SceneNode::SetChildrenShaderToMine()
{
	for(vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->sceneNodeShader = this->sceneNodeShader;
	}
}


void SceneNode::PreDraw(OGLRenderer &r)
{
	if(sceneNodeShader)
	{
		//if(!r.GetCurrentShader()->GetProgram() == sceneNodeShader->GetProgram())  //Fix this
		{
			r.SetCurrentShader(sceneNodeShader);


			SetSpecificUniforms(r);
			
		}
	}

}

void SceneNode::Draw(OGLRenderer &r)	{
	if(mesh)	
	{
			mesh->Draw();
	}
}

void SceneNode::SetSpecificUniforms(OGLRenderer & r)
{
		Matrix4 transform = worldTransform * Matrix4::Scale(modelScale);
		glUniformMatrix4fv(glGetUniformLocation(sceneNodeShader->GetProgram(), "modelMatrix"),	1,false, (float*)&transform);
		glUniformMatrix4fv(glGetUniformLocation(sceneNodeShader->GetProgram(), "viewMatrix") ,	1,false, (float*)&r.GetViewMatrix());
		glUniformMatrix4fv(glGetUniformLocation(sceneNodeShader->GetProgram(), "projMatrix") ,	1,false, (float*)&r.GetProjMatrix());

		glUniform4fv(glGetUniformLocation(sceneNodeShader->GetProgram(),"nodeColour"),1,(float*)&colour);
		glUniform1i(glGetUniformLocation(sceneNodeShader->GetProgram(), ""),(int)mesh->GetTexture());

		//glUniformMatrix4fv(glGetUniformLocation(sceneNodeShader->GetProgram(), "textureMatrix")  ,1,false, (float*)&textureMatrix);
}

void SceneNode::Update(float msec)	{
	if(parent)	{
		worldTransform = parent->worldTransform * transform;
	} else	{
		worldTransform = transform;
	}

	for(vector<SceneNode*>::iterator i = children.begin();
									 i != children.end(); ++i)	{
		(*i)->Update(msec);
	}
}