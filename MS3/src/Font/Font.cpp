#include "Font.h"

namespace Azul
{
	Font::Font(char* newStr, Glyph::Name newName)
		:
		pNext(nullptr),
		pPrev(nullptr),
		name(Font::Name::NOT_INITIALIZED),
		glyphName(newName),
		str(newStr)
	{
	}
	void Font::setString(const char* newStr)
	{
		this->str = newStr;
	}

	//-----links----
	void Font::setNext(Font* newNext)
	{
		this->pNext = newNext;
	}

	void Font::setPrev(Font* newPrev)
	{
		this->pPrev = newPrev;
	}

	Font* Font::getNext() const
	{
		return (this->pNext);
	}

	Font* Font::getPrev() const
	{
		return (this->pPrev);
	}

	Font::Name Font::getName() const
	{
		return this->name;
	}

	void Font::setName(Font::Name inName)
	{
		this->name = inName;
	}
}