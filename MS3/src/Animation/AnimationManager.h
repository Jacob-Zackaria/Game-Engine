#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "AnimationController.h"
#include "AnimationTransition.h"

namespace Azul
{
	class AnimationManager
	{
	public:
		static void Add(AnimationController* pCamera, AnimationController::Name name);
		static AnimationController* Find(AnimationController::Name name);
		static void Create();
		static void Destroy();
		static void Update();
		static void AddClip(Clip* pClip);
		static Clip* GetClip(Clip::Name clipName);
		static void AddSkeleton(Skeleton* pSkeleton);
		static void AddTransition(AnimationTransition* pTrans, AnimationTransition::Name newName);
		static AnimationTransition* Find(AnimationTransition::Name name);

	private:
		static AnimationManager* privGetInstance();
		AnimationManager();

		void privAddSkelFront(Skeleton* pNewData, Skeleton*& pHead);

		void privAddTransFront(AnimationTransition* pNewData, AnimationTransition*& pHead);


		void privAddToFront(AnimationController* pNewData, AnimationController*& pHead);
		void privRemove(AnimationController* pNewData, AnimationController*& pHead);

		// for clip.
		void privAddClipFront(Clip* pNewData, Clip*& pHead);
		void privRemoveClip(Clip* pNewData, Clip*& pHead);
	private:
		AnimationController* pControllerHead;
		Clip* pClipHead;
		Skeleton* pSkelHead;
		AnimationTransition* pAnimTransHead;
	};
}
#endif