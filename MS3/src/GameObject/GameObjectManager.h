//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "GameObject.h"
#include "PCSTree.h"
#include "AnimTime.h"
namespace Azul
{

	// Singleton
	class GameObjectManager
	{
	public:
		static void Add(GameObject* pObj, GameObject* pParent);
		static void Draw(void);
		static void Update(AnimTime currentTime);
		static GameObject* GetRoot(void);
		static PCSTree* GetPCSTree();

		//------------------
		static void SwapTexture();
		//------------------

		static void Create();
		static void Destroy();
		GameObjectManager(const GameObjectManager&) = delete;
		GameObjectManager& operator = (const GameObjectManager&) = delete;
		~GameObjectManager();

	private:
		GameObjectManager();

		static GameObjectManager* privGetInstance();

		// data
		PCSTree* poRootTree;
	};

}

#endif


// --- End of File ---
