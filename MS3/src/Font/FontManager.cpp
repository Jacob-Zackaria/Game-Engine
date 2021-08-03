#include "FontManager.h"

namespace Azul
{

	void FontManager::Create()
	{
		// Get the instance to the manager
		FontManager* pFontMan = FontManager::privGetInstance();
		assert(pFontMan);
		AZUL_UNUSED_VAR(pFontMan);
	}

	void FontManager::Destroy()
	{
		FontManager* pFontMan = FontManager::privGetInstance();
		assert(pFontMan);

		Font* pLink = pFontMan->fontList;

		while (pLink != nullptr)
		{
			Font* pTmp = pLink;
			pLink = (Font*)pLink->getNext();
			pFontMan->privRemove(pTmp, pFontMan->fontList);
			delete pTmp;
		}
	}

	void FontManager::Add(Font* pFont, Font::Name name)
	{
		// Get the instance to the manager
		FontManager* pFontMan = FontManager::privGetInstance();
		assert(pFontMan);


		assert(pFont);
		pFont->setName(name);

		// Now add it to the manager
		pFontMan->privAddToFront(pFont, pFontMan->fontList);
	}

	Font* FontManager::Find(Font::Name name)
	{
		// Get the instance to the manager
		FontManager* pFontMan = FontManager::privGetInstance();
		assert(pFontMan);

		Font* pLink = pFontMan->fontList;

		while (pLink != nullptr)
		{
			if (pLink->getName() == name)
			{
				return pLink;
			}
			pLink = (Font*)pLink->getNext();
		}

		return nullptr;
	}

	FontManager::FontManager()
	{
	}

	FontManager* FontManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static FontManager fontMan;
		return &fontMan;
	}

	void FontManager::privAddToFront(Font* pNewData, Font*& pHead)
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

	void FontManager::privRemove(Font* pNewData, Font*& pHead)
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