//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_SkinConstantColor.h"
#include "Model.h"
#include "CamerasManager.h"
#include "HumanModel.h"

namespace Azul
{
	GraphicsObject_SkinConstantColor::GraphicsObject_SkinConstantColor(const Model* const _pModel, const ShaderObject* const _pShaderObj, Vect& newColor, Vect& newLight, GameObjectRigid* pNewGameRigid)
		: GraphicsObject(_pModel, _pShaderObj)
	{
		this->color = new Vect(newColor);
		this->light = new Vect(newLight);
		this->pGameRigid = pNewGameRigid;

		// check the base class
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->poWorld);
	}

	GraphicsObject_SkinConstantColor::~GraphicsObject_SkinConstantColor()
	{
		delete this->color;
		delete this->light;
	}

	void GraphicsObject_SkinConstantColor::SetState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void GraphicsObject_SkinConstantColor::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		Camera* pCam = CamerasManager::GetActive3DCam();
		assert(pCam);

		Matrix view = pCam->getViewMatrix();
		Matrix proj = pCam->getProjMatrix();

		HumanModel* pHumanModel = (HumanModel*)this->GetModel();

		// Bine the SBOs
		pHumanModel->poInversePose_SBO->Bind(0);
		this->pBoneWord_SBO->Bind(1);
		// Send over Uniforms

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&view);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("local_matrix"), 1, GL_FALSE, (float*)pGameRigid->GetWorld());

		Vect plight = *this->light;
		Vect pcolor = *this->color;
		glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float*)&plight);
		glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float*)&pcolor);
	}

	void GraphicsObject_SkinConstantColor::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_SkinConstantColor::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}

//--- End of File ----

