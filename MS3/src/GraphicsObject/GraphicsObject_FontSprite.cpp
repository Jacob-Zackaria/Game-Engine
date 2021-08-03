//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_FontSprite.h"
#include "TextureManager.h"
#include "Model.h"
#include "Camera.h"
#include "CamerasManager.h"
#include "Image.h"
#include "GlyphManager.h"

namespace Azul
{
	GraphicsObject_FontSprite::GraphicsObject_FontSprite(Model* model, ShaderObject* pShaderObj, Font* _pFont, Rectangle rect)
		: GraphicsObject(model, pShaderObj)
	{
		assert(_pFont);
		this->pFont = _pFont;
		this->pGlyph = nullptr;
		this->pScreenRect = { rect.x, rect.y, rect.width, rect.height };
		this->origX = rect.x;
		this->origY = rect.y;
	}

	GraphicsObject_FontSprite::~GraphicsObject_FontSprite()
	{
	}

	void GraphicsObject_FontSprite::SetState()
	{
		float xTmp = this->origX;
		float yTmp = this->origY;
		float xEnd = this->origX;

		unsigned int i = 0;
		while (this->pFont->str[i] != '\0')
		{
			int findKey = (int)this->pFont->str[i];

			this->pGlyph = GlyphManager::Find(this->pFont->glyphName, findKey);
			assert(pGlyph);

			xTmp = xEnd + (this->pGlyph->rect.width / 2.0f);
			this->pScreenRect = { xTmp, yTmp, pGlyph->rect.width, pGlyph->rect.height };


			this->SwapData();
			this->pSetState();
			this->pSetDataGPU();
			this->pDraw();
			this->pRestoreState();
			
			xEnd = (this->pGlyph->rect.width / 2.0f) + xTmp;

			i++;
		}
	}

	void GraphicsObject_FontSprite::SwapData()
	{
		// Create the UV matrix
		this->poMatrix_uv = new Matrix();

		float w = this->pGlyph->rect.width / (float)this->pGlyph->pTex->width;
		float h = this->pGlyph->rect.height / (float)this->pGlyph->pTex->height;
		float u = this->pGlyph->rect.x / (float)this->pGlyph->pTex->width;
		float v = this->pGlyph->rect.y / (float)this->pGlyph->pTex->height;

		Matrix ScaleUV(Matrix::Scale::XYZ, w, h, 1.0f);
		Matrix TransUV(Matrix::Trans::XYZ, u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;

		// Create the orig Matrix 
		// The initial sprite setup, after this point standard world will adjust
		this->poMatrix_orig = new Matrix();

		Matrix Scale(Matrix::Scale::XYZ, this->pScreenRect.width, this->pScreenRect.height, 1.0f);
		Matrix Trans(Matrix::Trans::XYZ, this->pScreenRect.x, this->pScreenRect.y, 0.0f);
		*this->poMatrix_orig = Scale *Trans;
	}

	void GraphicsObject_FontSprite::pSetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = this->pGlyph->pTex->textureID;
		glBindTexture(GL_TEXTURE_2D, textureID);

		// comment out one
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);

		// Blends for sprites
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	}

	void GraphicsObject_FontSprite::SetDataGPU()
	{
	}

	void GraphicsObject_FontSprite::pSetDataGPU()
	{
		// Get camera
		Camera* pCam = CamerasManager::GetActive2DCam();

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		// use this shader
		this->pShaderObj->SetActive();

		// World matrix ----------------------------
		Matrix TransToOriginLowerLeft(Matrix::Trans::XYZ, (float)-pCam->getScreenWidth() / 2.0f, (float)-pCam->getScreenHeight() / 2.0f, 0.0f);
		Matrix tmpMatrix = *this->poMatrix_orig * this->GetWorld()* TransToOriginLowerLeft;

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&pCam->getProjMatrix());
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&pCam->getViewMatrix());
		glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&tmpMatrix);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("uv_matrix"), 1, GL_FALSE, (float*)this->poMatrix_uv);
	}

	void GraphicsObject_FontSprite::Draw()
	{
	}

	void GraphicsObject_FontSprite::pDraw()
	{
		// draw
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);   //The starting point of the IBO
	}

	void GraphicsObject_FontSprite::RestoreState()
	{
	}

	void GraphicsObject_FontSprite::pRestoreState()
	{
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		delete this->poMatrix_uv;
		delete this->poMatrix_orig;
	}
	
}

// --- End of File ---
