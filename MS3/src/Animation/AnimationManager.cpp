#include "AnimationManager.h"
#include "Clip.h"

namespace Azul
{
	AnimationManager::AnimationManager()
		:
		pControllerHead(nullptr),
		pClipHead(nullptr),
		pSkelHead(nullptr),
		pAnimTransHead(nullptr)
	{
	}

	void AnimationManager::Create()
	{
		AnimationManager* pAnimMan;
		pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);
	}

	void AnimationManager::Destroy()
	{
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		AnimationController* pLink = pAnimMan->pControllerHead;

		while (pLink != nullptr)
		{
			AnimationController* pTmp = pLink;
			pLink = (AnimationController*)pLink->getNext();
			pAnimMan->privRemove(pTmp, pAnimMan->pControllerHead);
			delete pTmp;
		}
		
		Clip* pNewLink = pAnimMan->pClipHead;

		while (pNewLink != nullptr)
		{
			Clip* pTmp = pNewLink;
			pNewLink = (Clip*)pNewLink->getNext();
			pAnimMan->privRemoveClip(pTmp, pAnimMan->pClipHead);
			delete pTmp;
		}

		Skeleton* pNewSLink = pAnimMan->pSkelHead;
		Skeleton* pTmp;
		while (pNewSLink != nullptr)
		{
			pTmp = (Skeleton*)pNewSLink->getNext();
			delete pNewSLink;
			pNewSLink = pTmp;
		}

		AnimationTransition* pNewTLink = pAnimMan->pAnimTransHead;
		AnimationTransition* pTmp2;
		while (pNewTLink != nullptr)
		{
			pTmp2 = (AnimationTransition*)pNewTLink->getNext();
			delete pNewTLink;
			pNewTLink = pTmp2;
		}
	}

	void AnimationManager::Update()
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		AnimationController* pAnimContol = pAnimMan->pControllerHead;

		while (pAnimContol != nullptr)
		{
			pAnimContol->Update();
			pAnimContol = (AnimationController*)pAnimContol->getNext();
		}
	}

	void AnimationManager::AddClip(Clip* pClip)
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		assert(pClip);

		// Now add it to the manager
		pAnimMan->privAddClipFront(pClip, pAnimMan->pClipHead);
	}

	Clip* AnimationManager::GetClip(Clip::Name clipName)
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		Clip* pLink = pAnimMan->pClipHead;

		while (pLink != nullptr)
		{
			if (pLink->getName() == clipName)
			{
				return pLink;
			}
			pLink = (Clip*)pLink->getNext();
		}

		return nullptr;
	}

	void AnimationManager::AddSkeleton(Skeleton* pSkeleton)
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		assert(pSkeleton);

		// Now add it to the manager
		pAnimMan->privAddSkelFront(pSkeleton, pAnimMan->pSkelHead);
	}

	void AnimationManager::AddTransition(AnimationTransition* pTrans, AnimationTransition::Name newName)
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		assert(pTrans);
		pTrans->setName(newName);

		// Now add it to the manager
		pAnimMan->privAddTransFront(pTrans, pAnimMan->pAnimTransHead);
	}

	void AnimationManager::Add(AnimationController* pController, AnimationController::Name name)
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		

		assert(pController);
		pController->setName(name);

		// Now add it to the manager
		pAnimMan->privAddToFront(pController, pAnimMan->pControllerHead);
	}

	AnimationController* AnimationManager::Find(AnimationController::Name name)
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		AnimationController* pLink = pAnimMan->pControllerHead;

		while (pLink != nullptr)
		{
			if (pLink->getName() == name)
			{
				return pLink;
			}
			pLink = (AnimationController*)pLink->getNext();
		}

		return nullptr;
	}

	AnimationTransition* AnimationManager::Find(AnimationTransition::Name name)
	{
		// Get the instance to the manager
		AnimationManager* pAnimMan = AnimationManager::privGetInstance();
		assert(pAnimMan);

		AnimationTransition* pLink = pAnimMan->pAnimTransHead;

		while (pLink != nullptr)
		{
			if (pLink->getName() == name)
			{
				return pLink;
			}
			pLink = (AnimationTransition*)pLink->getNext();
		}

		return nullptr;
	}


	AnimationManager* AnimationManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static AnimationManager animMan;
		return &animMan;
	}

	void AnimationManager::privAddToFront(AnimationController* pNewData, AnimationController*& pHead)
	{
		//Check if data to add is not null.
		assert(pNewData != nullptr);

		//If head is null.
		if (pHead == nullptr)
		{
			pNewData->setNext(nullptr);
			pNewData->setPrev(nullptr);
		}
		else        //If head is not null.
		{
			pNewData->setNext(pHead);
			pNewData->setPrev(nullptr);
			pHead->setPrev(pNewData);
		}

		//Assign head as the added data.
		pHead = pNewData;
	}

	void AnimationManager::privAddSkelFront(Skeleton* pNewData, Skeleton*& pHead)
	{
		//Check if data to add is not null.
		assert(pNewData != nullptr);

		//If head is null.
		if (pHead == nullptr)
		{
			pNewData->setNext(nullptr);
			pNewData->setPrev(nullptr);
		}
		else        //If head is not null.
		{
			pNewData->setNext(pHead);
			pNewData->setPrev(nullptr);
			pHead->setPrev(pNewData);
		}

		//Assign head as the added data.
		pHead = pNewData;
	}

	void AnimationManager::privAddTransFront(AnimationTransition* pNewData, AnimationTransition*& pHead)
	{
		//Check if data to add is not null.
		assert(pNewData != nullptr);

		//If head is null.
		if (pHead == nullptr)
		{
			pNewData->setNext(nullptr);
			pNewData->setPrev(nullptr);
		}
		else        //If head is not null.
		{
			pNewData->setNext(pHead);
			pNewData->setPrev(nullptr);
			pHead->setPrev(pNewData);
		}

		//Assign head as the added data.
		pHead = pNewData;
	}

	void AnimationManager::privRemove(AnimationController* pNewData, AnimationController*& pHead)
	{
		//Check if node to delete has a next or prev links.
		if (pNewData->getNext() == nullptr && pNewData->getPrev() == nullptr)
		{
			pHead = nullptr;
		}
		else if (pNewData->getPrev() == nullptr)    //Check if there is a prev link. 
		{
			pHead = pHead->getNext();
			pHead->setPrev(nullptr);
		}
		else if (pNewData->getNext() == nullptr)    //Check if there is a next link.
		{
			pNewData->getPrev()->setNext(nullptr);
		}
		else                    //If no next or prev links are present.
		{
			pNewData->getPrev()->setNext(pNewData->getNext());
			pNewData->getNext()->setPrev(pNewData->getPrev());
		}

		//Clear links of deleted node.
		pNewData->setNext(nullptr);
		pNewData->setPrev(nullptr);

	}

	void AnimationManager::privAddClipFront(Clip* pNewData, Clip*& pHead)
	{
		//Check if data to add is not null.
		assert(pNewData != nullptr);

		//If head is null.
		if (pHead == nullptr)
		{
			pNewData->setNext(nullptr);
			pNewData->setPrev(nullptr);
		}
		else        //If head is not null.
		{
			pNewData->setNext(pHead);
			pNewData->setPrev(nullptr);
			pHead->setPrev(pNewData);
		}

		//Assign head as the added data.
		pHead = pNewData;
	}

	void AnimationManager::privRemoveClip(Clip* pNewData, Clip*& pHead)
	{
		//Check if node to delete has a next or prev links.
		if (pNewData->getNext() == nullptr && pNewData->getPrev() == nullptr)
		{
			pHead = nullptr;
		}
		else if (pNewData->getPrev() == nullptr)    //Check if there is a prev link. 
		{
			pHead = pHead->getNext();
			pHead->setPrev(nullptr);
		}
		else if (pNewData->getNext() == nullptr)    //Check if there is a next link.
		{
			pNewData->getPrev()->setNext(nullptr);
		}
		else                    //If no next or prev links are present.
		{
			pNewData->getPrev()->setNext(pNewData->getNext());
			pNewData->getNext()->setPrev(pNewData->getPrev());
		}

		//Clear links of deleted node.
		pNewData->setNext(nullptr);
		pNewData->setPrev(nullptr);

	}
}