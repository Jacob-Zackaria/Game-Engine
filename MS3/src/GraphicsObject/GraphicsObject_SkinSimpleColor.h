//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_SKIN_SIMPLE_COLOR_H
#define GRAPHICS_OBJECT_SKIN_SIMPLE_COLOR_H

#include "GraphicsObject.h"
#include "SSBO.h"
#include "GameObjectRigid.h"
namespace Azul
{
	class GraphicsObject_SkinSimpleColor :public GraphicsObject
	{
	public:
		GraphicsObject_SkinSimpleColor(const Model* const pModel, const ShaderObject* const pShaderObj, Vect& newColor, GameObjectRigid* pNewGameRigid);

		GraphicsObject_SkinSimpleColor() = delete;
		GraphicsObject_SkinSimpleColor(const GraphicsObject_SkinSimpleColor&) = delete;
		GraphicsObject_SkinSimpleColor& operator = (const GraphicsObject_SkinSimpleColor&) = delete;
		~GraphicsObject_SkinSimpleColor();

	
		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		SSBO* pBoneWord_SBO;
		Vect* color;
		GameObjectRigid* pGameRigid;
	};
}

#endif

//--- End of File ---
