#include "TextureManager.h"
#include "TGA.h"
#include "sb7.h"

namespace Azul
{

	TextureManager::TextureManager()
		:
		active(nullptr)
	{
	}

	TextureManager* TextureManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static TextureManager textMan;
		return &textMan;
	}

	void TextureManager::Create()
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();
		assert(pTextMan);
		AZUL_UNUSED_VAR(pTextMan);
	}

	void TextureManager::Destroy()
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();
		assert(pTextMan);

		TextureLink* pLink = pTextMan->active;
		TextureLink* pTmp = nullptr;
		while (pLink != nullptr)
		{
			pTmp = pLink;
			pLink = pLink->next;

			delete pTmp;
		}

	}

	//update reference count.
	void TextureManager::Load(const Texture::Name _name)
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();
		assert(pTextMan);

		Texture* pLink = (Texture*)pTextMan->active;
		while (pLink != nullptr)
		{
			if (pLink->name == _name)
			{
				pLink->rCount++;
				break;
			}
			pLink = (Texture*)pLink->next;
		}

		if (pLink == nullptr)
		{
			switch (_name)
			{
			case Texture::Name::DUCKWEED:
				pTextMan->Add("./Assets/Duckweed.tga", Texture::Name::DUCKWEED);
				break;
			case Texture::Name::RED_BRICK:
				pTextMan->Add("./Assets/RedBrick.tga", Texture::Name::RED_BRICK);
				break;
			case Texture::Name::ROCKS:
				pTextMan->Add("./Assets/Rocks.tga", Texture::Name::ROCKS);
				break;
			case Texture::Name::STONES:
				pTextMan->Add("./Assets/Stone.tga", Texture::Name::STONES);
				break;
			case Texture::Name::DEFAULT:
				pTextMan->Add("./Assets/HotPink.tga", Texture::Name::DEFAULT);
				break;
			case Texture::Name::SPACE_SHIP:
				pTextMan->Add("./Assets/space_frigate.tga", Texture::Name::SPACE_SHIP);
				break;
			case Texture::Name::ASTRO_BOY:
				pTextMan->Add("./Assets/astroboy.tga", Texture::Name::ASTRO_BOY);
				break;
			case Texture::Name::MILITARY_TRUCK:
				pTextMan->Add("./Assets/MilitaryTruck.tga", Texture::Name::MILITARY_TRUCK);
				break;
			case Texture::Name::WAR_BEAR:
				pTextMan->Add("./Assets/war_bear.tga", Texture::Name::WAR_BEAR);
				break;
			case Texture::Name::POLAR_BEAR:
				pTextMan->Add("./Assets/polarbear.tga", Texture::Name::POLAR_BEAR);
				break;
			case Texture::Name::MISSILE:
				pTextMan->Add("./Assets/missileTexture.png", Texture::Name::MISSILE);
				break;
			case Texture::Name::INVADERS:
				pTextMan->Add("./Assets/Aliens.tga", Texture::Name::INVADERS);
				break;
			case Texture::Name::CONSOLAS_FONT:
				pTextMan->Add("./Assets/Consolas36pt.tga", Texture::Name::CONSOLAS_FONT);
				break;
			case Texture::Name::BRADLEY_FONT:
				pTextMan->Add("./Assets/Bradley_30pt.tga", Texture::Name::BRADLEY_FONT);
				break;
			case Texture::Name::TEDDY:
				pTextMan->Add("./Assets/TeddyTexNORLE.tga", Texture::Name::TEDDY);
				break;
			case Texture::Name::NOT_INITIALIZED:
				break;
			}
			pTextMan->Load(_name);
		}
	}



	void TextureManager::UnLoad(const Texture::Name _name)
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();
		assert(pTextMan);

		Texture* pLink = (Texture*)pTextMan->active;
		while (pLink != nullptr)
		{
			if (pLink->name == _name)
			{
				pLink->rCount -= 1;
				if (pLink->rCount <= 0)
				{
					pTextMan->privRemove(pLink, pTextMan->active);
					pTextMan->DeleteTexture(pLink->textureID);
					delete pLink;
				}
				break;
			}
			pLink = (Texture*)pLink->next;
		}
	}

	void TextureManager::Remove(const Texture::Name _name)
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();
		assert(pTextMan);

		Texture* pLink = (Texture*)pTextMan->active;
		while (pLink != nullptr)
		{
			if (pLink->name == _name)
			{
				pTextMan->privRemove(pLink, pTextMan->active);
				pTextMan->DeleteTexture(pLink->textureID);
				delete pLink;
				break;
			}
			pLink = (Texture*)pLink->next;
		}
	}

	void TextureManager::Dump()
	{
		Trace::out("\n\n---------Texture Manager--------------------\n");

		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();
		assert(pTextMan);

		Texture* pLink = (Texture*)pTextMan->active;
		unsigned int i = 0;
		while (pLink != nullptr)
		{
			Trace::out("\n----------------------------\n");
			Trace::out("\n%d::\n", i++);
			pLink->Dump();
			Trace::out("\n----------------------------\n");
			pLink = (Texture*)pLink->next;
		}

		Trace::out("\n--------------------------------------------\n");
	}

	void TextureManager::privRemove(TextureLink* pNewData, TextureLink*& pHead)
	{
		//Check if node to delete has a next or prev links.
		if (pNewData->next == nullptr && pNewData->prev == nullptr)
		{
			pHead = nullptr;
		}
		else if (pNewData->prev == nullptr)    //Check if there is a prev link. 
		{
			pHead = pHead->next;
			pHead->prev = nullptr;
		}
		else if (pNewData->next == nullptr)    //Check if there is a next link.
		{
			pNewData->prev->next = nullptr;
		}
		else                    //If no next or prev links are present.
		{
			pNewData->prev->next = pNewData->next;
			pNewData->next->prev = pNewData->prev;
		}

		//Clear links of deleted node.
		pNewData->next = nullptr;
		pNewData->prev = nullptr;
	}

	

	void TextureManager::Add(const char* const _assetName, const Texture::Name _name)
	{
		GLuint               textureID = 0;
		GLuint* pTextureID = &textureID;

		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();

		// Load the texture and get the textureID
		int width;
		int height;
		pTextMan->privLoadTexture(_assetName, pTextureID, width, height);

		// Create a TextureNode
		Texture* pNode = new Texture();

		// initialize it
		pNode->set(_assetName, _name, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);

		// Now add it to the manager
		pTextMan->privAddToFront(pNode, pTextMan->active);
	}

	void TextureManager::AddBuff(const char* const buffer, const char* const asset_name, const Texture::Name _name)
	{
		GLuint               textureID = 0;
		GLuint* pTextureID = &textureID;

		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();

		// Load the texture and get the textureID
		int width;
		int height;
		pTextMan->privLoadBuffTexture(buffer, pTextureID, width, height);

		// Create a TextureNode
		Texture* pNode = new Texture();

		// initialize it
		pNode->set(asset_name, _name, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);

		// Now add it to the manager
		pTextMan->privAddToFront(pNode, pTextMan->active);
	}

	Texture* TextureManager::Find(const Texture::Name _name)
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();

		Texture* pNode = (Texture*)pTextMan->active;
		while (pNode != 0)
		{
			if (pNode->name == _name)
			{
				// found it
				break;
			}

			pNode = (Texture*)pNode->next;
		}

		return pNode;
	}

	GLuint TextureManager::FindID(const Texture::Name _name)
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();

		Texture* pNode = (Texture*)pTextMan->active;
		while (pNode != 0)
		{
			if (pNode->name == _name)
			{
				// found it
				break;
			}

			pNode = (Texture*)pNode->next;
		}

		return pNode->textureID;
	}


	void TextureManager::DeleteAllTextures()
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::privGetInstance();

		Texture* pNode = (Texture*)pTextMan->active;
		while (pNode != 0)
		{
			// bye bye
			glDeleteTextures(1, &pNode->textureID);

			// next
			pNode = (Texture*)pNode->next;
		}
	}

	void TextureManager::DeleteTexture(unsigned int pNewID)
	{
		glDeleteTextures(1, &pNewID);
	}

	void TextureManager::privAddToFront(TextureLink* node, TextureLink*& head)
	{
		assert(node);

		if (head == nullptr)
		{
			head = node;
			node->next = nullptr;
			node->prev = nullptr;
		}
		else
		{
			node->next = head;
			head->prev = node;
			head = node;
		}
	}

	void TextureManager::privLoadTexture(const char* const _assetName, GLuint*& textureID, int& width, int& height)
	{
		// Get an ID, for textures (OpenGL poor man's handle)
		glGenTextures(1, textureID);

		// Bind it.
		glBindTexture(GL_TEXTURE_2D, *textureID);

		// Loat the texture
		this->privLoadTGATexture(_assetName, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
	}

	void TextureManager::privLoadBuffTexture(const char* const buffer, GLuint*& textureID, int& width, int& height)
	{
		// Get an ID, for textures (OpenGL poor man's handle)
		glGenTextures(1, textureID);

		// Bind it.
		glBindTexture(GL_TEXTURE_2D, *textureID);

		// Loat the texture
		this->privLoadTGABuffTexture(buffer, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
	}

	// Load a TGA as a 2D Texture. Completely initialize the state
	bool TextureManager::privLoadTGATexture(const char* szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& nWidth, int& nHeight)
	{
		GLbyte* pBits;
		int nComponents;
		GLenum eFormat;

		// Read the texture bits
		pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
		if (pBits == nullptr)
		{
			return false;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizWrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

		free(pBits);

		if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
			minFilter == GL_LINEAR_MIPMAP_NEAREST ||
			minFilter == GL_NEAREST_MIPMAP_LINEAR ||
			minFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		return true;
	}

	// Load a TGA as a 2D Texture. Completely initialize the state
	bool TextureManager::privLoadTGABuffTexture(const char* buffer, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& nWidth, int& nHeight)
	{
		GLbyte* pBits;
		int nComponents;
		GLenum eFormat;

		// Read the texture bits
		pBits = gltReadTGABuffBits(buffer, &nWidth, &nHeight, &nComponents, &eFormat);
		if (pBits == nullptr)
		{
			return false;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizWrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

		free(pBits);

		if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
			minFilter == GL_LINEAR_MIPMAP_NEAREST ||
			minFilter == GL_NEAREST_MIPMAP_LINEAR ||
			minFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		return true;
	}

	////////////////////////////////////////////////////////////////////
	// Allocate memory and load targa bits. Returns pointer to new buffer,
	// height, and width of texture, and the OpenGL format of data.
	// Call free() on buffer when finished!
	// This only works on pretty vanilla targas... 8, 24, or 32 bit color
	// only, no palettes, no RLE encoding.
	GLbyte* TextureManager::gltReadTGABits(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat)
	{
		FILE* pFile;			// File pointer
		TGAHEADER tgaHeader;		// TGA file header
		unsigned long lImageSize;		// Size in bytes of image
		short sDepth;			// Pixel depth;
		GLbyte* pBits = NULL;          // Pointer to bits

		// Default/Failed values
		*iWidth = 0;
		*iHeight = 0;
		*eFormat = GL_RGB;
		*iComponents = GL_RGB;

		// Attempt to open the file
		fopen_s(&pFile, szFileName, "rb");

		if (pFile == nullptr)
		{
			assert(false);
			return nullptr;
		}

		// Read in header (binary)
		fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);


		// Get width, height, and depth of texture
		*iWidth = tgaHeader.width;
		*iHeight = tgaHeader.height;
		sDepth = tgaHeader.bits / 8;

		// Put some validity checks here. Very simply, I only understand
		// or care about 8, 24, or 32 bit targa's.
		if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
			return nullptr;

		// Calculate size of image buffer
		lImageSize = (unsigned int)tgaHeader.width * (unsigned int)tgaHeader.height * (unsigned int)sDepth;

		// Allocate memory and check for success
		pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
		if (pBits == nullptr)
			return nullptr;

		// Read in the bits
		// Check for read error. This should catch RLE or other 
		// weird formats that I don't want to recognize
		if (fread(pBits, lImageSize, 1, pFile) != 1)
		{
			free(pBits);
			return nullptr;
		}

		// Set OpenGL format expected
		switch (sDepth)
		{

		case 3:     // Most likely case
			*eFormat = GL_BGR;
			*iComponents = GL_RGB;
			break;

		case 4:
			*eFormat = GL_BGRA;
			*iComponents = GL_RGBA;
			break;
		case 1:
			assert(false);
			// bad case - keenan
			// i commented out
		  //  *eFormat = GL_LUMINANCE;
		  //  *iComponents = GL_LUMINANCE;
			break;
		default:        // RGB
			// If on the iPhone, TGA's are BGR, and the iPhone does not 
			// support BGR without alpha, but it does support RGB,
			// so a simple swizzle of the red and blue bytes will suffice.
			// For faster iPhone loads however, save your TGA's with an Alpha!

			break;
		}

		// Done with File
		fclose(pFile);

		// Return pointer to image data
		return pBits;
	}

	GLbyte* TextureManager::gltReadTGABuffBits(const char* buffer, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat)
	{
		TGAHEADER tgaHeader;		// TGA file header
		unsigned long lImageSize;		// Size in bytes of image
		short sDepth;			// Pixel depth;
		GLbyte* pBits = NULL;          // Pointer to bits

		// Default/Failed values
		*iWidth = 0;
		*iHeight = 0;
		*eFormat = GL_RGB;
		*iComponents = GL_RGB;

		// Read in header (binary)
		//fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
		memcpy_s(&tgaHeader, 18, buffer, 18);

		// Get width, height, and depth of texture
		*iWidth = tgaHeader.width;
		*iHeight = tgaHeader.height;
		sDepth = tgaHeader.bits / 8;

		// Put some validity checks here. Very simply, I only understand
		// or care about 8, 24, or 32 bit targa's.
		if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
			return nullptr;

		// Calculate size of image buffer
		lImageSize = (unsigned int)tgaHeader.width * (unsigned int)tgaHeader.height * (unsigned int)sDepth;

		// Allocate memory and check for success
		pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
		if (pBits == nullptr)
			return nullptr;

		// Read in the bits
		// Check for read error. This should catch RLE or other 
		// weird formats that I don't want to recognize
		/*if (fread(pBits, lImageSize, 1, pFile) != 1)
		{
			free(pBits);
			return nullptr;
		}*/
		if (memcpy_s(pBits, lImageSize, (buffer + 18), lImageSize))
		{
			free(pBits);
			return nullptr;
		}

		// Set OpenGL format expected
		switch (sDepth)
		{

		case 3:     // Most likely case
			*eFormat = GL_BGR;
			*iComponents = GL_RGB;
			break;

		case 4:
			*eFormat = GL_BGRA;
			*iComponents = GL_RGBA;
			break;
		case 1:
			assert(false);
			// bad case - keenan
			// i commented out
		  //  *eFormat = GL_LUMINANCE;
		  //  *iComponents = GL_LUMINANCE;
			break;
		default:        // RGB
			// If on the iPhone, TGA's are BGR, and the iPhone does not 
			// support BGR without alpha, but it does support RGB,
			// so a simple swizzle of the red and blue bytes will suffice.
			// For faster iPhone loads however, save your TGA's with an Alpha!

			break;
		}

		// Return pointer to image data
		return pBits;
	}
}


// --- End of File -----

