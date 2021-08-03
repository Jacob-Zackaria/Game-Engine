#include "MathEngine.h"
#include "Model.h"
#include "GraphicsObject_ColorByPosition.h"
#include "CamerasManager.h"

using namespace Azul;

namespace Azul
{
	GraphicsObject_ColorByPosition::GraphicsObject_ColorByPosition(const Model* const model, const ShaderObject* const pShaderObj)
		: GraphicsObject(model, pShaderObj)
	{
		assert(pModel);
		assert(pShaderObj);
		assert(poWorld);
	}

	void GraphicsObject_ColorByPosition::SetState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

	void GraphicsObject_ColorByPosition::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		Matrix world = this->GetWorld();
		Matrix view = CamerasManager::GetActive3DCam()->getViewMatrix();
		Matrix proj = CamerasManager::GetActive3DCam()->getProjMatrix();

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&view);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&world);
	}

	void GraphicsObject_ColorByPosition::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_ColorByPosition::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}