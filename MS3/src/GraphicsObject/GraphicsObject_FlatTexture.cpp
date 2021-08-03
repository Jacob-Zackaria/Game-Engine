//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_FlatTexture.h"
#include "TextureManager.h"
#include "Model.h"
#include "CamerasManager.h"



namespace Azul
{
	GraphicsObject_FlatTexture::GraphicsObject_FlatTexture(const Model* const _pModel, const ShaderObject* const _pShaderObj, Texture::Name _name, Draw_Mode d_mode)
		: GraphicsObject(_pModel, _pShaderObj), textName(_name), draw_mode(d_mode)
	{
		// update ref count.
		TextureManager::Load(this->textName);
		// check the base class
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->poWorld);
	}

	GraphicsObject_FlatTexture::~GraphicsObject_FlatTexture()
	{
		TextureManager::UnLoad(this->textName);
	}

	void GraphicsObject_FlatTexture::SetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = TextureManager::FindID(this->textName);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(this->draw_mode);
	}

	void GraphicsObject_FlatTexture::SetDataGPU()
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
		glUniform1i(this->pShaderObj->GetLocation("tex_object"), 0);
	}

	void GraphicsObject_FlatTexture::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_FlatTexture::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(this->draw_mode);
	}

}

//--- End of File -------------------------------------------------------------

