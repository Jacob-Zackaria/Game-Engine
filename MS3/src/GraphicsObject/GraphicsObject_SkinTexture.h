//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_SKIN_TEXTURE_H
#define GRAPHICS_OBJECT_SKIN_TEXTURE_H

#include "GraphicsObject.h"
#include "Texture.h"
#include "SSBO.h"
#include "GameObjectRigid.h"
namespace Azul
{
	class GraphicsObject_SkinTexture :public GraphicsObject
	{
	public:
		GraphicsObject_SkinTexture(const Model *const pModel, const ShaderObject *const pShaderObj, Texture::Name _name, GameObjectRigid* pNewGameRigid);

		GraphicsObject_SkinTexture() = delete;
		GraphicsObject_SkinTexture(const GraphicsObject_SkinTexture &) = delete;
		GraphicsObject_SkinTexture &operator = (const GraphicsObject_SkinTexture &) = delete;
		~GraphicsObject_SkinTexture() = default;

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		Texture::Name textName;
		SSBO* pBoneWord_SBO;
		GameObjectRigid* pGameRigid;
	};
}

#endif

//--- End of File ---
