//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "GameObjectAnimation.h"
#include "GameObject.h"
#include "Model.h"

namespace Azul
{
	
	GameObjectAnimation::GameObjectAnimation(GraphicsObject* pGraphicsObject, Bone* _pBoneResult)
		: GameObject(pGraphicsObject),
		pBoneResult(_pBoneResult)
	{
		assert(pGraphicsObject);
		assert(pBoneResult);

		this->poDof = new Vect(0.0f, 0.0f, 0.0f);
		assert(poDof);

		this->poUp = new Vect(0.0f, 1.0f, 0.0f);
		assert(poUp);

		this->poScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(poScale);

		this->poPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(poPos);

		this->indexBoneArray = 0;

		this->poLocal = new Matrix(Matrix::Special::Identity);
		assert(poLocal);

		this->poBoneOrientation = new Matrix(Matrix::Special::Identity);
		assert(poBoneOrientation);
	}

	GameObjectAnimation::~GameObjectAnimation()
	{
		delete this->poDof;
		delete this->poUp;
		delete this->poScale;
		delete this->poPos;

		delete this->poLocal;
		delete this->poBoneOrientation;
	}

	void GameObjectAnimation::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		GameObject* pBoneParent = (GameObject*)this->GetParent();
		assert(pBoneParent);

		Matrix ParentWorld = *pBoneParent->GetWorld();

		// REMEMBER this is for Animation and hierachy, you need to handle models differently
		// Get the result bone array, from there make the matrix
		Matrix T = Matrix(Matrix::Trans::XYZ, this->pBoneResult[indexBoneArray].T);
		Matrix S = Matrix(Matrix::Scale::XYZ, this->pBoneResult[indexBoneArray].S);
		Quat   Q = this->pBoneResult[indexBoneArray].Q;

		// Isn't it awesome that we can multiply Quat with matrices!
		Matrix M = S * Q * T;
		*this->poLocal = M;

		// Goal: update the world matrix
		*this->poWorld = *this->poLocal * ParentWorld;

	}

	void GameObjectAnimation::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->baseUpdateBoundingSphere();

		// push to graphics object
		Matrix mTmp = *this->poBoneOrientation;

		// push to graphics object
		this->poGraphicsObject->SetWorld(mTmp);
	}

	void GameObjectAnimation::SetIndex(int val)
	{
		this->indexBoneArray = val;
	}

	void GameObjectAnimation::SetBoneOrientation(const Matrix& tmp)
	{
		*this->poBoneOrientation = tmp;
	}

	Matrix GameObjectAnimation::GetBoneOrientation(void) const
	{
		return Matrix(*this->poBoneOrientation);
	}
}

// --- End of File ---