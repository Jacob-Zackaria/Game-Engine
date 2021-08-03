//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Animation.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectManager.h"
#include "Clip.h"
#include "AnimationManager.h"

namespace Azul
{
	Animation::Animation(Clip::Name _name)
		: 
		pClip(AnimationManager::GetClip(_name)),
		numBones(pClip->GetNumOfBones()),
		boneWidth(pClip->GetBoneWidth())	
	{
		/*this->poBoneResult = new Bone[(unsigned int)this->numBones]();
		assert(this->poBoneResult);*/


		assert(pClip);
	}

	Animation::~Animation()
	{
		//delete[] this->poBoneResult;
	}

	AnimTime Animation::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}


	void Animation::Animate(AnimTime tCurr)
	{
		this->pClip->AnimateBones(tCurr);
	}

	void Animation::SetClip(Clip::Name _name)
	{
		this->pClip = AnimationManager::GetClip(_name);
		this->numBones = this->pClip->GetNumOfBones();
		this->boneWidth = this->pClip->GetBoneWidth();
	}

	void Animation::privSetBonePose(GameObjectAnimation* node)
	{
		// Now get the world matrices
		// getting the parent from current node
		GameObjectAnimation* childNode = (GameObjectAnimation*)node;
		GameObjectAnimation* parentNode;
		if (node->GetParent()->GetParent()->GetParent() != nullptr)
		{
			parentNode = (GameObjectAnimation*)node->GetParent();
		}
		else
		{
			parentNode = nullptr;
		}

		PCSTree* pTree = GameObjectManager::GetPCSTree();
		assert(pTree);
		if (parentNode == pTree->GetRoot())
			return;

		if (parentNode != nullptr && childNode != nullptr)
		{
			// starting point
			Vect start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//  Now get the length and directions

			Vect ptA = start  * *parentNode->GetWorld();
			Vect ptB = start  * *childNode->GetWorld();

			// direction between the anchor points of the respective bones
			Vect dir = (ptA - ptB);

			// length of the bone 0
			float mag = dir.mag();

			Matrix S(Matrix::Scale::XYZ, this->boneWidth, this->boneWidth, mag);
			Quat Q(Quat::Orient::LocalToWorld, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
			Matrix T(Matrix::Trans::XYZ, ptB);

			Matrix BoneOrient = S * Q * T;

			childNode->SetBoneOrientation(BoneOrient);
		}
	}


}

//--- End of File ----