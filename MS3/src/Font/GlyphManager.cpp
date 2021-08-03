#include "GlyphManager.h"
#include "File.h"
namespace Azul
{
	
	void GlyphManager::Create()
	{
		// Get the instance to the manager
		GlyphManager* pGlyphMan = GlyphManager::privGetInstance();
		assert(pGlyphMan);
		AZUL_UNUSED_VAR(pGlyphMan);
	}

	void GlyphManager::Destroy()
	{
		GlyphManager* pFontMan = GlyphManager::privGetInstance();
		assert(pFontMan);

		Glyph* pLink = pFontMan->glyphList;

		while (pLink != nullptr)
		{
			Glyph* pTmp = pLink;
			pLink = (Glyph*)pLink->getNext();
			pFontMan->privRemove(pTmp, pFontMan->glyphList);
			delete pTmp;
		}
	}

	void GlyphManager::Add(Glyph* pGlyph, Glyph::Name name)
	{
		// Get the instance to the manager
		GlyphManager* pGlyphMan = GlyphManager::privGetInstance();
		assert(pGlyphMan);

		assert(pGlyph);
		
		pGlyph->setName(name);

		// Now add it to the manager
		pGlyphMan->privAddToFront(pGlyph, pGlyphMan->glyphList);
	}

	Glyph* GlyphManager::Find(Glyph::Name name, int findKey)
	{
		// Get the instance to the manager
		GlyphManager* pGlyphMan = GlyphManager::privGetInstance();
		assert(pGlyphMan);

		Glyph* pLink = pGlyphMan->glyphList;

		while (pLink != nullptr)
		{
			if (pLink->getName() == name && findKey == pLink->key)
			{
				return pLink;
			}
			pLink = (Glyph*)pLink->getNext();
		}

		return nullptr;
	}

	void GlyphManager::AddXml(Glyph::Name glyphName, const char* assetName, Texture::Name textName)
	{
		// Get the instance to the manager
		GlyphManager* pGlyphMan = GlyphManager::privGetInstance();
		assert(pGlyphMan);

		// File stuff
		File::Handle fh;
		File::Error  status;
		custom_font cf = { 0 };
		DWORD currentSize = 0, fileSize = 0;

		// READ the data from the file ONLY
		status = File::Open(fh, assetName, File::Mode::READ);
		assert(status == File::Error::SUCCESS);

		// move file pointer to end of file
		status = File::Seek(fh, File::Location::END, 0);
		assert(status == File::Error::SUCCESS);

		// get the total size.
		status = File::Tell(fh, fileSize);
		assert(status == File::Error::SUCCESS);

		// move file pointer to beginning of file
		status = File::Seek(fh, File::Location::BEGIN, 0);
		assert(status == File::Error::SUCCESS);

		while (currentSize < fileSize)
		{
			// Read the Hdr
			status = File::Read(fh, &cf, sizeof(custom_font));
			assert(status == File::Error::SUCCESS);

			// get the current size.
			status = File::Tell(fh, currentSize);
			assert(status == File::Error::SUCCESS);

			Glyph* newGlyph = new Glyph();
			newGlyph->set(textName, cf.key, cf.x, cf.y, cf.width, cf.height);
			pGlyphMan->Add(newGlyph, glyphName);
		}
		
		// close file.
		status = File::Close(fh);
		assert(status == File::Error::SUCCESS);
	}

	GlyphManager::GlyphManager()
	{
	}

	GlyphManager* GlyphManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static GlyphManager fontMan;
		return &fontMan;
	}

	void GlyphManager::privAddToFront(Glyph* pNewData, Glyph*& pHead)
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

	void GlyphManager::privRemove(Glyph* pNewData, Glyph*& pHead)
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