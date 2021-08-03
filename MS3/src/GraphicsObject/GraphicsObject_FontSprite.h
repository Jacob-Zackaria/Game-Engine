//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_FONT_SPRITE_H
#define GRAPHICS_OBJECT_FONT_SPRITE_H

#include "Texture.h"
#include "Image.h"
#include "Font.h"
#include "GraphicsObject.h"
#include "ShaderObject.h"
namespace Azul
{
	class GraphicsObject_FontSprite : public GraphicsObject
	{
	public:
		GraphicsObject_FontSprite(Model* pModel, ShaderObject* pShaderObj, Font* pFont, Rectangle rect);
		virtual ~GraphicsObject_FontSprite();

		// Rendermaterial contract
		virtual void SetState() override;
		void pSetState();
		virtual void SetDataGPU() override;
		void pSetDataGPU();
		virtual void Draw() override;
		void pDraw();
		virtual void RestoreState() override;
		void pRestoreState();

		void SwapData();

		// data:  place uniform instancing here

			//GLuint textureID;
		Font* pFont;
		Glyph* pGlyph;
		Matrix* poMatrix_uv;
		Matrix* poMatrix_orig;
		Rectangle pScreenRect;
		float origX;
		float origY;
	};
}

#endif

//--- End of File ---

