//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "NullModel.h"
#include "GraphicsObject_Null.h"
#include "GraphicsObject_FlatTexture.h"
#include "GameObjectRigid.h"
#include "GameObject.h"
#include "GameObjectAnimation.h"
#include "GameObjectManager.h"
#include "GameObjectNull.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "PCSTreeForwardIterator.h"

namespace Azul
{
	void GameObjectManager::Add(GameObject* pObj, GameObject* pParent)
	{
		assert(pObj != nullptr);
		assert(pParent != nullptr);

		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();

		// insert object to root
		pGOM->poRootTree->Insert(pObj, pParent);

	}

	void GameObjectManager::Create()
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);
		AZUL_UNUSED_VAR(pGOM);
	}

	PCSTree* GameObjectManager::GetPCSTree()
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		// Get root node
		return pGOM->poRootTree;
	}

	GameObject* GameObjectManager::GetRoot()
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		GameObject* pGameObj = (GameObject*)pGOM->poRootTree->GetRoot();
		assert(pGameObj);

		return pGameObj;
	}

	void GameObjectManager::Destroy()
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSTree* pTree = pGOM->poRootTree;
		PCSNode* pNode = nullptr;

		PCSTreeForwardIterator pForIter(pTree->GetRoot());
		pNode = pForIter.First();
		PCSNode* pTmp = nullptr;
		while (!pForIter.IsDone())
		{
			pTmp = pForIter.CurrentItem();

			pNode = pForIter.Next();
			delete pTmp;

		}


	}

	void GameObjectManager::Update(AnimTime currentTime)
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		// PCSTreeForwardIterator pIter(pRootNode->GetChild());
		PCSTreeForwardIterator pIter(pRootNode);
		PCSNode* pNode = pIter.First();

		GameObject* pGameObj = nullptr;

		while (!pIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject*)pNode;
			pGameObj->Update(currentTime);

			pNode = pIter.Next();
		}
	}

	//------------------
	void GameObjectManager::SwapTexture()
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pForwardIter(pRootNode);
		PCSNode* pNode = pForwardIter.First();

		GameObject* pGameObj = nullptr;

		while (!pForwardIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject*)pNode;
			GraphicsObject *pnewObj = pGameObj->GetGraphicsObject();
			ShaderObject* newShader = const_cast<ShaderObject*>(pnewObj->GetShaderObject());

			if (newShader->name == ShaderObject::Name::TEXTURE_SIMPLE)
			{
				GraphicsObject_FlatTexture* newft = static_cast<GraphicsObject_FlatTexture*>(pnewObj);

				if (newft->textName == Texture::Name::WAR_BEAR)
				{
					newft->textName = Texture::Name::POLAR_BEAR;
				}
				else if (newft->textName == Texture::Name::POLAR_BEAR)
				{
					newft->textName = Texture::Name::WAR_BEAR;
				}
			}
			
			pNode = pForwardIter.Next();
		}
	}
	//------------------
	void GameObjectManager::Draw()
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pForwardIter(pRootNode);
		PCSNode* pNode = pForwardIter.First();

		GameObject* pGameObj = 0;

		while (!pForwardIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject*)pNode;
			if (pGameObj->GetDrawEnable())
			{
				pGameObj->Draw();
			}
			else
			{
				//assert(0);
			}

			pNode = pForwardIter.Next();
		}
	}

	GameObjectManager::GameObjectManager()
	{
		// Create the root node (null object)
		NullModel* pModel = new NullModel(nullptr);
		assert(pModel);
		ModelManager::Add(pModel);

		ShaderObject* pShader = new ShaderObject(ShaderObject::Name::NULL_SHADER, "./Shaders/nullRender", ShaderObject::Type::GRAPHICS);
		assert(pShader);
		ShaderManager::Add(pShader);

		GraphicsObject_Null* pGraphicsObject = new GraphicsObject_Null(pModel, pShader);
	
		GameObjectNull* pGameRoot = new GameObjectNull(pGraphicsObject);
		pGameRoot->SetName("GameObjectNull_Root");

		// Create the tree
		this->poRootTree = new PCSTree();
		assert(this->poRootTree);

		// Attach the root node
		this->poRootTree->Insert(pGameRoot, this->poRootTree->GetRoot());
	}

	GameObjectManager::~GameObjectManager()
	{
		delete this->poRootTree;
	}

	GameObjectManager* GameObjectManager::privGetInstance(void)
	{
		// This is where its actually stored (BSS section)
		static GameObjectManager gom;
		return &gom;
	}

}

// --- End of File ---
