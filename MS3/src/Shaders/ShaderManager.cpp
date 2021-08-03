//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ShaderManager.h"

namespace Azul
{
	ShaderManager::ShaderManager()
	{
		this->active = nullptr;
	}

	ShaderManager* ShaderManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static ShaderManager shaderMan;
		return &shaderMan;
	}


	void ShaderManager::Create()
	{
		ShaderManager* pShaderMan;
		pShaderMan = ShaderManager::privGetInstance();
		assert(pShaderMan);
	}

	void ShaderManager::Destroy()
	{
		ShaderManager* pShaderMan = ShaderManager::privGetInstance();
		assert(pShaderMan);

		ShaderObject* pLink = pShaderMan->active;

		while (pLink != nullptr)
		{
			ShaderObject* pTmp = pLink;
			pLink = (ShaderObject*)pLink->next;
			pShaderMan->privRemove(pTmp, pShaderMan->active);
			delete pTmp;
		}
	}

	ShaderObject* ShaderManager::Find(ShaderObject::Name reqName)
	{
		ShaderManager* pShaderMan = ShaderManager::privGetInstance();
		assert(pShaderMan);

		ShaderObject* pLink = pShaderMan->active;

		while (pLink != nullptr)
		{
			if (pLink->name == reqName)
				break;

			pLink = (ShaderObject*)pLink->next;
		}

		return pLink;
	}

	void ShaderManager::Add(ShaderObject* pShader)
	{
		// Get the instance to the manager
		ShaderManager* pShaderMan = ShaderManager::privGetInstance();
		assert(pShaderMan);

		// Create a TextureNode
		assert(pShader);

		// Now add it to the manager
		pShaderMan->privAddToFront(pShader, pShaderMan->active);
	}

	void ShaderManager::privAddToFront(ShaderObject* node, ShaderObject*& head)
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

	void ShaderManager::privRemove(ShaderObject* pNode, ShaderObject*& poHead)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead = (ShaderObject*)pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}

	}

}

// ---  End of File ---
