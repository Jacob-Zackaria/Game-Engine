#ifndef FONT_MANAGER_H
#define FONT_MAANGER_H
#include "Font.h"

namespace Azul
{
	class FontManager
	{
	public:
		FontManager(const FontManager&) = delete;
		FontManager& operator = (const FontManager&) = delete;
		~FontManager() = default;

		static void Create();
		static void Destroy();
		static void Add(Font* pFont, Font::Name name);
		static Font* Find(Font::Name name);

	private:
		FontManager();
		static FontManager* privGetInstance();
		void privAddToFront(Font* pNewData, Font*& pHead);
		void privRemove(Font* pNewData, Font*& pHead);

		Font* fontList;
	};
}
#endif