#ifndef FONT_H
#define FONT_H
#include "Glyph.h"
namespace Azul
{
	class Font
	{
	public:
		enum class Name
		{
			CONSOLAS_FONT,
			BRADLEY_FONT,
			NOT_INITIALIZED
		};

		Font(char* newStr, Glyph::Name newName);
		~Font() = default;

		void setString(const char* newStr = "");

		//-----links----
		void setNext(Font* newNext);
		void setPrev(Font* newPrev);
		Font* getNext() const;
		Font* getPrev() const;

		Name getName() const;
		void setName(Font::Name name);
	public:
		// Links-----
		Font* pNext;
		Font* pPrev;

		// Name
		Font::Name name;
		Glyph::Name glyphName;
		const char* str;
	};
}

#endif