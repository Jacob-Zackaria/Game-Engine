//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIMATION_TRANSITION_H
#define ANIMATION_TRANSITION_H

#include "TimerController.h"
#include "GameObjectAnimation.h"
#include "Animation.h"
#include "HierarchyTable.h"

namespace Azul
{
	class AnimationTransition
	{
	public:
		enum class Name
		{
			TEDDY_WALK_IDLE,
			HUMAN_RUN_PUNCH,
			UNDEFINED
		};

		AnimationTransition(HierarchyTable* pNewTableData, Clip *pClip, Clip *pClipB, float blendTime);
		AnimationTransition(const AnimationTransition&) = delete;
		AnimationTransition& operator = (const AnimationTransition&) = delete;
		~AnimationTransition();

		void Update();
		void StartTransition();
		void privTransitionTime();
		void SetTime(float newTime);
		//-----links----
		void setNext(AnimationTransition* newNext);
		void setPrev(AnimationTransition* newPrev);
		AnimationTransition* getNext() const;
		AnimationTransition* getPrev() const;

		// accessors
		AnimationTransition::Name getName() const;
		void setName(AnimationTransition::Name name);
		void SetDelta(float newDelta);

	public:
		SSBO* pSSBO_A;
		SSBO* pSSBO_B;
		SSBO* poSSBO_Result;
		HierarchyTable* pTableData;
		SSBO* poBoneWorld_SBO;

		int numBones;
		float tS;
		bool StartEnable;
	private:
		float delta;
		AnimationTransition::Name pName;
		// Links-----
		AnimationTransition* pNext;
		AnimationTransition* pPrev;
	};
}

#endif

// --- End of File ---

