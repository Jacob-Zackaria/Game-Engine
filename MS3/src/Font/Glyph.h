#ifndef GLYPH_H
#define GLYPH_H
#include "Rectangle.h"
#include "Texture.h"
namespace Azul
{
	class Glyph
	{
	public:
		enum class Name
		{
			CONSOLAS_FONT,
			BRADLEY_FONT,
			NOT_INITIALIZED
		};

		Glyph();
		~Glyph() = default;

		void set(Texture::Name pTexName, int newkey, float x, float y, float width, float height);

		//-----links----
		void setNext(Glyph* newNext);
		void setPrev(Glyph* newPrev);
		Glyph* getNext() const;
		Glyph* getPrev() const;

		Name getName() const;
		void setName(Glyph::Name name);

	private:
		// Links-----
		Glyph* pNext;
		Glyph* pPrev;
	public:

		int key;
		Rectangle rect;
		Texture* pTex;
		// Name
		Glyph::Name name;
	};
}

#endif