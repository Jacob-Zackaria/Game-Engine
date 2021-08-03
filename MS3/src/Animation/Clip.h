//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "Bone.h"
#include "sb7.h"
#include "SSBO.h"
#include "AnimTimer.h"
namespace Azul
{
	class Clip
	{
	public:
		enum class Name
		{
			HUMANOID_RUN = 0,
			HUMANOID_PUNCH,
			HUMANOID_SHOT,

			TEDDY_WALK,
			TEDDY_IDLE,
			NOT_INITIALIZED
		};

		struct FrameBucket
		{
			FrameBucket();
			FrameBucket(const FrameBucket&) = default;
			FrameBucket& operator = (const FrameBucket&) = default;
			~FrameBucket();


			//-----links----
			void setNext(FrameBucket* newNext)
			{
				this->nextBucket = newNext;
			}
			void setPrev(FrameBucket* newPrev)
			{
				this->prevBucket = newPrev;
			}
			FrameBucket* getNext() const
			{
				return (this->nextBucket);
			}
			FrameBucket* getPrev() const
			{
				return (this->prevBucket);
			}



			FrameBucket* nextBucket;
			FrameBucket* prevBucket;
			AnimTime        KeyTime;
			SSBO* poKeyframe;
		};

	public:
		Clip(int _numBones, float _boneWidth, const char* _clipFile, Clip::Name _pName, int clipIndex);

		Clip() = delete;
		Clip(const Clip&) = delete;
		Clip& operator = (const Clip&) = delete;
		~Clip();

		AnimTime GetTotalTime();
		void AnimateBones(AnimTime tCurr);

		// accessors
		Clip::Name getName() const;
		void setName(Clip::Name name);

		int GetNumOfBones() const;
		float GetBoneWidth() const;

		//-----links----
		void setNext(Clip* newNext);
		void setPrev(Clip* newPrev);
		Clip* getNext() const;
		Clip* getPrev() const;

	private:
		void privSetAnimationData(const char* clipFile, int clipIndex);
		AnimTime privFindMaxTime();
		int  privFindNumFrames();
		void privAddToEnd(FrameBucket* node, FrameBucket*& head);

	private:
		int          numBones;
		int          numFrames;
		AnimTime         TotalTime;
		FrameBucket* poHead;
		Clip::Name pName;
		float boneWidth;

		// Links-----
		Clip* pNext;
		Clip* pPrev;

	public:
		SSBO* pSSBO_Result;
	};
}

#endif

//--- End of File ---