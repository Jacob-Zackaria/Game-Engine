//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_RIGID_H
#define GAME_OBJECT_RIGID_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

namespace Azul
{
	// Keenan(33)
	class GameObjectRigid : public GameObject
	{
	public:
		GameObjectRigid(GraphicsObject *graphicsObject);

		// Big four
		GameObjectRigid() = delete;
		GameObjectRigid(const GameObjectRigid &) = delete;
		GameObjectRigid &operator=(GameObjectRigid &) = delete;
		virtual ~GameObjectRigid() override;

		virtual void Update(AnimTime currentTime);

		void SetTrans(const Vect newTrans);

	private:
		void privUpdate(AnimTime currentTime);

	public:
		Vect poScale;
		Vect poTrans;
		float angle_x;
		float angle_y;
		float angle_z;
		//float pad[1];
	};
}

#endif

// --- End of File ---
