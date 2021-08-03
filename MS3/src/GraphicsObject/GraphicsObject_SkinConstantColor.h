//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_SKIN_CONSTANT_COLOR_H
#define GRAPHICS_OBJECT_SKIN_CONSTANT_COLOR_H

#include "GraphicsObject.h"
#include "SSBO.h"
#include "GameObjectRigid.h"
namespace Azul
{
	class GraphicsObject_SkinConstantColor :public GraphicsObject
	{
	public:
		GraphicsObject_SkinConstantColor(const Model* const pModel, const ShaderObject* const pShaderObj, Vect& newColor, Vect& newLight, GameObjectRigid* pNewGameRigid);

		GraphicsObject_SkinConstantColor() = delete;
		GraphicsObject_SkinConstantColor(const GraphicsObject_SkinConstantColor&) = delete;
		GraphicsObject_SkinConstantColor& operator = (const GraphicsObject_SkinConstantColor&) = delete;
		~GraphicsObject_SkinConstantColor();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		SSBO* pBoneWord_SBO;
		Vect* color;
		Vect* light;
		GameObjectRigid* pGameRigid;

	};
}

#endif

//--- End of File ---
