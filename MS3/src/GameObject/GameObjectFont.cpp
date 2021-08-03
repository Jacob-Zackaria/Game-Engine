//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectFont.h"
#include "GraphicsObject.h"

namespace Azul
{
	GameObjectFont::GameObjectFont(GraphicsObject_FontSprite* pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject != 0);

		this->angle = 0.0f;
		this->posX = pGraphicsObject->pScreenRect.x;
		this->posY = pGraphicsObject->pScreenRect.y;
		this->scaleX = 1.0f;
		this->scaleY = 1.0f;
	}

	GameObjectFont::~GameObjectFont()
	{
	}


	void GameObjectFont::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		Matrix Scale(Matrix::Scale::XYZ, this->scaleX, this->scaleY, 1.0f);
		Matrix Rot(Matrix::Rot1::Z, this->angle);
		Matrix Trans(Matrix::Trans::XYZ, this->posX, this->posY, 0.0f);

		*this->poWorld = Scale * Rot * Trans;

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

}

//--- End of File ---
