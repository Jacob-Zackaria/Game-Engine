//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GameObjectRigid.h"

namespace Azul
{
	GameObjectRigid::GameObjectRigid(GraphicsObject* pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject);

		this->poScale = Vect(1.0f, 1.0f, 1.0f);

		this->poTrans = Vect(0.0f, 0.0f, 0.0f);

		this->angle_x = 0.0f;
		this->angle_y = 0.0f;
		this->angle_z = 0.0f;
	}

	GameObjectRigid::~GameObjectRigid()
	{
		
	}

	void GameObjectRigid::privUpdate(AnimTime)
	{

		Matrix S(Matrix::Scale::XYZ, this->poScale);
		Matrix T(Matrix::Trans::XYZ, this->poTrans);
		Matrix Rx = Matrix(Matrix::Rot1::X, angle_x);
		Matrix Ry = Matrix(Matrix::Rot1::Y, angle_y);
		Matrix Rz = Matrix(Matrix::Rot1::Z, angle_z);
		Matrix M = S * Rx * Ry * Rz * T;

		*this->poWorld = M;
	}

	void GameObjectRigid::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->baseUpdateBoundingSphere();

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObjectRigid::SetTrans(const Vect newTrans)
	{
		this->poTrans = newTrans;
	}
}

// --- End of File ---