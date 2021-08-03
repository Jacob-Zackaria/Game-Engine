#include "Glyph.h"
#include "TextureManager.h"

namespace Azul
{
	Glyph::Glyph()
		:
		pNext(nullptr),
		pPrev(nullptr),
		key(0),
		rect{},
		pTex(nullptr),
		name(Glyph::Name::NOT_INITIALIZED)
	{
	}

	void Glyph::set(Texture::Name pTexName, int newkey, float x, float y, float width, float height)
	{
		this->pTex = TextureManager::Find(pTexName);
		assert(pTex);

		this->rect = {x, y , width, height};

		this->key = newkey;
	}

	//-----links----
	void Glyph::setNext(Glyph* newNext)
	{
		this->pNext = newNext;
	}

	void Glyph::setPrev(Glyph* newPrev)
	{
		this->pPrev = newPrev;
	}

	Glyph* Glyph::getNext() const
	{
		return (this->pNext);
	}

	Glyph* Glyph::getPrev() const
	{
		return (this->pPrev);
	}

	Glyph::Name Glyph::getName() const
	{
		return this->name;
	}

	void Glyph::setName(Glyph::Name inName)
	{
		this->name = inName;
	}
}