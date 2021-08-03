//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject3D.h"
#include "GraphicsObject.h"

namespace Azul
{
	GameObject3D::GameObject3D(GraphicsObject* pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject);
		this->poTrans = new Vect(0.0f, 0.0f, 0.0f);
		this->poScale = new Vect(1.0f, 1.0f, 1.0f);

		assert(this->poTrans);
		assert(this->poScale);

		this->rotX = 0.0f;
		this->rotY = 0.0f;
		this->rotZ = 0.0f;
		this->deltaRotZ = 0.0f;
		this->deltaRotY = 0.0f;
		this->deltaRotX = 0.0f;
	}

	GameObject3D::~GameObject3D()
	{
		delete this->poTrans;
		delete this->poScale;
	}

	void GameObject3D::setMove(MoveDir move)
	{
		this->moveDir = move;
	};

	void GameObject3D::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		float delta = 0.01f;

		static float dirX = 1.0f;
		static float dirY = 1.0f;
		static float dirZ = 1.0f;

		switch (this->moveDir)
		{
		case Move_X:
			this->poTrans->x() += (delta * dirX);
			if (this->poTrans->x() > 6.0f || this->poTrans->x() < -6.0f)
			{
				dirX *= -1.0f;
			}
			break;

		case Move_Y:
			this->poTrans->y() += (delta * dirY);
			if (this->poTrans->y() > 4.0f || this->poTrans->y() < -4.0f)
			{
				dirY *= -1.0f;
			}
			break;

		case Move_Z:
			this->poTrans->z() += (delta * dirZ);
			if (this->poTrans->z() > 7.0f || this->poTrans->z() < -5.0f)
			{
				dirZ *= -1.0f;
			}
			break;

		case Move_None:
		default:
			break;
		}

		// Goal: update the world matrix

		this->rotX += deltaRotX;
		this->rotY += deltaRotY;
		this->rotZ += deltaRotZ;

		Matrix Scale(Matrix::Scale::XYZ, *this->poScale);
		Matrix TransA(Matrix::Trans::XYZ, *this->poTrans);
		Matrix RotX(Matrix::Rot1::X, this->rotX);
		Matrix RotY(Matrix::Rot1::Y, this->rotY);
		Matrix RotZ(Matrix::Rot1::Z, this->rotZ);

		// world matrix
		*this->poWorld = Scale * RotX * RotY * RotZ * TransA;
	}

	void GameObject3D::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->baseUpdateBoundingSphere();

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObject3D::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObject3D::SetPos(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void GameObject3D::SetRot(float x, float y, float z)
	{
		this->deltaRotX = x;
		this->deltaRotY = y;
		this->deltaRotZ = z;
	}

	void GameObject3D::SetOrient(float x, float y, float z)
	{
		x = x;
		y = y;
		z = z;
	}
}

// --- End of File ---