//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_FONT_H
#define GAME_OBJECT_FONT_H

#include "MathEngine.h"
#include "GameObject.h"
#include "GraphicsObject_FontSprite.h"

namespace Azul
{
	class GameObjectFont : public GameObject
	{
	public:

		GameObjectFont(GraphicsObject_FontSprite* graphicsObject);

		// Big four
		GameObjectFont() = delete;
		GameObjectFont(const GameObjectFont&) = delete;
		GameObjectFont& operator=(GameObjectFont&) = delete;
		virtual ~GameObjectFont() override;


		virtual void Update(AnimTime currentTime) override;


	public:
		float scaleX;
		float scaleY;
		float posX;
		float posY;
		float angle;
	};
}

#endif

// --- End of File ---
