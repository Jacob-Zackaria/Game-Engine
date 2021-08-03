//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAMEOBJECT_3D_H
#define GAMEOBJECT_3D_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

namespace Azul
{
	class GameObject3D : public GameObject
	{
	public:
		enum MoveDir
		{
			Move_X,
			Move_Y,
			Move_Z,
			Move_None
		};

	public:
	public:
		GameObject3D(GraphicsObject* graphicsObject);

		// Big four
		GameObject3D() = delete;
		GameObject3D(const GameObject3D&) = delete;
		GameObject3D& operator=(GameObject3D&) = delete;
		virtual ~GameObject3D() override;

		virtual void Update(AnimTime currentTime);

		void SetScale(float sx, float sy, float sz);
		void SetPos(float x, float y, float z);
		void SetRot(float x, float y, float z);
		void SetOrient(float x, float y, float z);
		void setMove(MoveDir move);

	private:
		void privUpdate(AnimTime currentTime);

	public:   // add accessors later
		Vect* poScale;
		Vect* poTrans;

		float deltaRotX;
		float deltaRotY;
		float deltaRotZ;
		float rotX;
		float rotY;
		float rotZ;

		MoveDir moveDir;

	};

}

#endif

// --- End of File ---