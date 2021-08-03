//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BOUNDING_SPHERE_GAME_OBJECT_H
#define BOUNDING_SPHERE_GAME_OBJECT_H

#include "MathEngine.h"
#include "GameObject.h"
#include "AnimTime.h"
namespace Azul
{
	class BoundingSphereGameObject : public GameObject
	{
	public:
		BoundingSphereGameObject(GraphicsObject *graphicsObject);

		// Big four
		BoundingSphereGameObject() = delete;
		BoundingSphereGameObject(const BoundingSphereGameObject &) = delete;
		BoundingSphereGameObject &operator=(BoundingSphereGameObject &) = delete;
		virtual ~BoundingSphereGameObject();

		void SetBoundingSphereReference(GameObject *pGameObj);

		virtual void Draw() override;
		virtual void Update(AnimTime currentTime) override;
		void toggleBoundingSphere();

	private:
		void privUpdate(AnimTime currentTime);
		void privUpdateBoundingSphereReference(AnimTime currentTime);

		// Data: ------------------
		GameObject *pRefGameObj;
		bool toggleOff;
	public:
		Vect *poScale;
		Vect *poTrans;
	};

}

#endif

// --- End of File ---
