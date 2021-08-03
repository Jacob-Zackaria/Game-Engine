//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"
namespace Azul
{

	class GameObject : public PCSNode
	{
	public:
		GameObject() = delete;
		GameObject(const GameObject&) = delete;
		GameObject& operator = (const GameObject&) = delete;
		virtual ~GameObject();

		GameObject(GraphicsObject* graphicsObject);  

		virtual void Draw();
		virtual void Update(AnimTime currentTime) = 0;

		// Accessors
		GraphicsObject* GetGraphicsObject();
		Matrix* GetWorld();
		void SetWorld(Matrix* pWorld);
		bool GetDrawEnable();
		void SetDrawEnable(bool);

	protected:
		Matrix* poWorld;
		void baseUpdateBoundingSphere();

	protected:
		Sphere* pSphere;
		GraphicsObject* poGraphicsObject;
		bool mDrawEnable;
		char pad[3];
	};
}

#endif


// --- End of File ---
