//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_SkinTexture.h"
#include "TextureManager.h"
#include "Model.h"
#include "CamerasManager.h"
//#include "TeddyModel2.h"
#include "TeddyModel.h"

namespace Azul
{
	GraphicsObject_SkinTexture::GraphicsObject_SkinTexture(const Model *const _pModel, const ShaderObject *const _pShaderObj, Texture::Name _name, GameObjectRigid* pNewGameRigid)
		: GraphicsObject(_pModel, _pShaderObj), textName(_name), pGameRigid(pNewGameRigid)
	{
		// check the base class
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->poWorld);
	}

	void GraphicsObject_SkinTexture::SetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = TextureManager::FindID(this->textName);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void GraphicsObject_SkinTexture::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		Camera* pCam = CamerasManager::GetActive3DCam();
		assert(pCam);

		Matrix view = pCam->getViewMatrix();
		Matrix proj = pCam->getProjMatrix();

		TeddyModel* pTeddyModel = (TeddyModel*)this->GetModel();
		
		// Bine the SBOs
		pTeddyModel->poInversePose_SBO->Bind(0);
		this->pBoneWord_SBO->Bind(1);
		// Send over Uniforms

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("local_matrix"), 1, GL_FALSE, (float*)pGameRigid->GetWorld());

		// Texture - linked to Texture 0
		glUniform1i(this->pShaderObj->GetLocation("tex_object"), 0);
	}

	void GraphicsObject_SkinTexture::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_SkinTexture::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}

//--- End of File ----

