//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimationController.h"
#include "Animation.h"

namespace Azul
{
	AnimationController::AnimationController(AnimTime delta, Clip::Name _name)
		:
		pName(AnimationController::Name::NOT_INITIALIZED)
	{
		// Animation object
		this->poAnim = new Animation( _name);
		assert(this->poAnim);

		// TimerController
		AnimTime maxTime = poAnim->FindMaxTime();
		AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
		this->poTimerControl = new TimerController(delta, minTime, maxTime);
		assert(this->poTimerControl);

	}

	AnimationController::~AnimationController()
	{
		delete poAnim;
		delete poTimerControl;
	}

	void AnimationController::Update()
	{
		this->poTimerControl->Update();

		// interpolate all bones
		this->poAnim->Animate(this->poTimerControl->GetCurrTime());
	}

	AnimationController::Name AnimationController::getName() const
	{
		return this->pName;
	}

	void AnimationController::setName(AnimationController::Name inName)
	{
		this->pName = inName;
	}

	void AnimationController::SetTimer(const AnimTime newTime)
	{
		this->poTimerControl->SetDeltaTime(newTime);
	}

	AnimTime AnimationController::GetTimer() const
	{
		return this->poTimerControl->GetDeltaTime();
	}

	void AnimationController::SetClip(Clip::Name _name)
	{
		this->poAnim->SetClip(_name);
	}


	/*void AnimationController::SetStart(const Vect newPos)
	{
		this->poAnim->SetStartPos(newPos);
	}*/

	//-----links----

	void AnimationController::setNext(AnimationController* newNext)
	{
		this->pNext = newNext;
	}

	void AnimationController::setPrev(AnimationController* newPrev)
	{
		this->pPrev = newPrev;
	}

	AnimationController* AnimationController::getNext() const
	{
		return (this->pNext);
	}

	AnimationController* AnimationController::getPrev() const
	{
		return (this->pPrev);
	}
}

// --- End of File ---