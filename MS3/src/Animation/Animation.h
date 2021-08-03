//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimTimer.h"
#include "GameObjectAnimation.h"
#include "Clip.h"
namespace Azul
{

	class Animation
	{
	public:
		Animation(Clip::Name _name);
		Animation(const Animation&) = delete;
		Animation& operator = (const Animation&) = delete;
		~Animation();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		// set StartPos.
		void SetClip(Clip::Name _name);

	private:
		void privSetBonePose(GameObjectAnimation* node);

		// hack
	public:
		Clip* pClip;	
	//	Bone* poBoneResult;

	private:
		int   numBones;
		float boneWidth;
	};


}

#endif

// --- End of File ---