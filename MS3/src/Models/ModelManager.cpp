//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "ModelManager.h"

namespace Azul
{    
	ModelManager::ModelManager()
	{  
		this->active = nullptr;
	} 

	ModelManager* ModelManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static ModelManager shaderMan;
		return &shaderMan;
	}

	void ModelManager::Create()
	{
		ModelManager* pModelMan;
		pModelMan = ModelManager::privGetInstance();
		assert(pModelMan);
	}

	void ModelManager::Destroy()
	{
		ModelManager* pModelMan = ModelManager::privGetInstance();
		assert(pModelMan);

		Model* pLink = pModelMan->active;

		while (pLink != nullptr)
		{
			Model* pTmp = pLink;
			pLink = (Model*)pLink->next;
			pModelMan->privRemove(pTmp, pModelMan->active);
			delete pTmp;
		}
	}

	void ModelManager::Add(Model* pModel)
	{
		// Get the instance to the manager
		ModelManager* pModelMan = ModelManager::privGetInstance();
		assert(pModelMan);

		// Create a Model
		assert(pModel);

		// Now add it to the manager
		pModelMan->privAddToFront(pModel, pModelMan->active);
	}

	void ModelManager::privAddToFront(Model* node, Model*& head)
	{
		assert(node != 0);

		if (head == 0)
		{
			head = node;
			node->next = 0;
			node->prev = 0;
		}
		else
		{
			node->next = head;
			head->prev = node;
			head = node;
		}
	}

	void ModelManager::privRemove(Model* pNode, Model*& poHead)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead = (Model*)pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}

	}

}

// ---  End of File ---
