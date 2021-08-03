#include "Model.h"
#include "GraphicsObject_Wireframe.h"
#include "ShaderObject.h"
#include "CamerasManager.h"


namespace Azul
{

	GraphicsObject_Wireframe::GraphicsObject_Wireframe(const Model* const _pModel, const ShaderObject* const _pShaderObj)
		: GraphicsObject(_pModel, _pShaderObj)
	{
		assert(pModel);
		assert(pShaderObj);
		assert(poWorld);
	}

	void GraphicsObject_Wireframe::SetState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

	void GraphicsObject_Wireframe::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// Draw Me: -----------------------------------------

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		// Set state
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

		Matrix world = this->GetWorld();
		Matrix view = CamerasManager::GetActive3DCam()->getViewMatrix();
		Matrix proj = CamerasManager::GetActive3DCam()->getProjMatrix();

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&view);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&world);
	}

	void GraphicsObject_Wireframe::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_Wireframe::RestoreState()
	{
		// Restore state
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}