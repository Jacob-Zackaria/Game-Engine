
//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Clip.h"
#include "Time.h"
#include "Bone.h"
#include "Mixer.h"
#include "File.h"
#include "ShaderManager.h"
#include "TeddyModel.h"
#include "AnimationController.h"
#include "GameObjectRigid.h"
namespace Azul
{
	// Get from the manager
	Clip::FrameBucket::FrameBucket()
		:nextBucket(nullptr),
		prevBucket(nullptr),
		KeyTime(),
		//poBone(nullptr),
		poKeyframe(nullptr)
	{
	}

	Clip::FrameBucket::~FrameBucket()
	{
		//delete[] poBone;
		delete poKeyframe;
	}

	Clip::Clip(int _numBones, float _boneWidth, const char* _clipFile, Clip::Name _pName, int clipIndex)
		: numBones(_numBones),
		numFrames(0),
		TotalTime(AnimTime::Duration::ZERO),
		poHead(nullptr),
		pName(_pName),
		boneWidth(_boneWidth),
		pSSBO_Result(nullptr)
	{
		this->privSetAnimationData(_clipFile, clipIndex);
		this->TotalTime = this->privFindMaxTime();
		this->numFrames = this->privFindNumFrames();
		this->pSSBO_Result = new SSBO();

		// Only done once per clip
		Bone* pBone = new Bone[(unsigned int)numBones]();
		assert(pBone);
		this->pSSBO_Result->Set((unsigned int)numBones, sizeof(Bone), pBone);
		delete[] pBone;
	}

	Clip::~Clip()
	{
		FrameBucket* pTmp = poHead;

		while (pTmp != nullptr)
		{
			FrameBucket* pDeleteMe = pTmp;
			pTmp = pTmp->nextBucket;
			delete pDeleteMe;
		}
		delete this->pSSBO_Result;
	}

	void Clip::AnimateBones(AnimTime tCurr)
	{
		// First one 
		FrameBucket* pTmp = this->poHead;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket* pA = pTmp->prevBucket;
		FrameBucket* pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		Mixer::Blend_GPU(pA->poKeyframe, pB->poKeyframe, this->pSSBO_Result, tS, numBones);
	}

	void Clip::privSetAnimationData(const char* clipFile, int clipIndex)
	{
		struct NewVect
		{
			NewVect(float _x, float _y, float _z)
				:x(_x), y(_y), z(_z)
			{
			}
			float x;
			float y;
			float z;
		};

		File::Handle clipHandle;
		File::Error status;
		status = File::Open(clipHandle, clipFile, File::Mode::READ);
		assert(status == File::Error::SUCCESS);

		// Read number of bones.

		//name * 3(T,Q,S) * Vect size * number of bones * number of frames.
		int move = clipIndex * 3 * (int)sizeof(NewVect) * this->numBones * 31;
		status = File::Seek(clipHandle, File::Location::BEGIN, move);
		assert(status == File::Error::SUCCESS);

		NewVect *newVect = new NewVect(0.0f, 0.0f, 0.0f);

		Bone* pBone = new Bone[(unsigned int)this->numBones]();
		
		for (int i = 0; i < 31; i++)
		{
			FrameBucket* pTmp = new FrameBucket();
			pTmp->KeyTime = i * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
			pTmp->poKeyframe = new SSBO();
			assert(pTmp->poKeyframe);

			for (int j = 0; j < this->numBones; j++)
			{
				// Translation read.
				status = File::Read(clipHandle, newVect, sizeof(NewVect));
				assert(status == File::Error::SUCCESS);
				pBone[j].T = Vect(newVect->x, newVect->y, newVect->z);

				// Rotation read.
				status = File::Read(clipHandle, newVect, sizeof(NewVect));
				assert(status == File::Error::SUCCESS);
				pBone[j].Q = Quat(Quat::Rot3::XYZ, newVect->x, newVect->y, newVect->z);

				// Scale read.
				status = File::Read(clipHandle, newVect, sizeof(NewVect));
				assert(status == File::Error::SUCCESS);
				pBone[j].S = Vect(newVect->x, newVect->y, newVect->z);
			}
			pTmp->poKeyframe->Set((unsigned int)this->numBones, sizeof(Bone), &pBone[0]);

			this->privAddToEnd(pTmp, this->poHead);
		}

		delete[] pBone;

		status = File::Close(clipHandle);
		assert(status == File::Error::SUCCESS);

		delete newVect;
	}

	Clip::Name Clip::getName() const
	{
		return this->pName;
	}

	void Clip::setName(Clip::Name inName)
	{
		this->pName = inName;
	}

	int Clip::GetNumOfBones() const
	{
		return this->numBones;
	}

	float Clip::GetBoneWidth() const
	{
		return this->boneWidth;
	}

	//-----links----

	void Clip::setNext(Clip* newNext)
	{
		this->pNext = newNext;
	}

	void Clip::setPrev(Clip* newPrev)
	{
		this->pPrev = newPrev;
	}

	Clip* Clip::getNext() const
	{
		return (this->pNext);
	}

	Clip* Clip::getPrev() const
	{
		return (this->pPrev);
	}

	int Clip::privFindNumFrames()
	{
		int count = 0;
		FrameBucket* pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			count++;
			pTmp = pTmp->nextBucket;
		}
		return count;
	}

	AnimTime Clip::privFindMaxTime()
	{
		AnimTime tMax;
		FrameBucket* pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;

		return tMax;
	}

	AnimTime Clip::GetTotalTime()
	{
		return this->TotalTime;
	}

	void Clip::privAddToEnd(FrameBucket* node, FrameBucket*& head)
	{
		assert(node);

		if (head == nullptr)
		{
			head = node;
			node->setPrev(nullptr);
		}
		else
		{
			FrameBucket* pTmp = head;
			while (pTmp->getNext() != nullptr)
			{
				pTmp = pTmp->getNext();
			}
			node->setPrev(pTmp);
			pTmp->setNext(node);
		}
		node->setNext(nullptr);
	}
}



// --- End of File ---