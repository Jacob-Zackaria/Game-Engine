//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIMATION_H
#define GAME_OBJECT_ANIMATION_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"
#include "Bone.h"
namespace Azul
{
	class GameObjectAnimation : public GameObject
	{
	public:
		GameObjectAnimation(GraphicsObject* graphicsObject, Bone* pBoneResult);

		// Big four
		GameObjectAnimation() = delete;
		GameObjectAnimation(const GameObjectAnimation&) = delete;
		GameObjectAnimation&operator=(GameObjectAnimation&) = delete;
		virtual ~GameObjectAnimation() override;

		virtual void Update(AnimTime currentTime);

		// Accessors
		Matrix GetBoneOrientation(void) const;
		void SetBoneOrientation(const Matrix &);
		void SetIndex(int val);

	private:
		void privUpdate(AnimTime currentTime);

	public:
		Vect *poScale;
		Vect *poPos;
		Vect *poDof;
		Vect *poUp;
	
		Matrix *poLocal;
		Matrix *poBoneOrientation;
		Bone* pBoneResult;
		int    indexBoneArray;
	};

}

#endif

// --- End of File ---
