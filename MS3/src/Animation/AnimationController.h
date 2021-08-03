//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include "TimerController.h"
#include "GameObjectAnimation.h"
#include "Animation.h"
#include "Skeleton.h"
namespace Azul
{
	class AnimationController
	{
	public:
		enum class Name
		{
			HUMANOID_1,
			HUMANOID_2,
			TEDDY,
			NOT_INITIALIZED
		};

		AnimationController(AnimTime delta, Clip::Name _name);
		AnimationController(const AnimationController&) = delete;
		AnimationController&operator = (const AnimationController&) = delete;
		~AnimationController();

		void Update();

		// accessors
		AnimationController::Name getName() const;
		void setName(AnimationController::Name name);

		void SetTimer(const AnimTime);
		AnimTime GetTimer() const;

		void SetClip(Clip::Name _name);

		//-----links----
		void setNext(AnimationController* newNext);
		void setPrev(AnimationController* newPrev);
		AnimationController* getNext() const;
		AnimationController* getPrev() const;

	private:
		TimerController *poTimerControl;
		//HACK
	public:
		Animation* poAnim;
		AnimationController::Name pName;

		// Links-----
		AnimationController* pNext;
		AnimationController* pPrev;
	};
}

#endif

// --- End of File ---
