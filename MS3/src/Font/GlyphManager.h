#ifndef GLYPH_MANAGER_H
#define GLYPH_MANAGER_H
#include "Glyph.h"

namespace Azul
{
	// custom font structure //
	struct custom_font
	{
		int key = 0;
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
	};

	class GlyphManager
	{
	public:
		GlyphManager(const GlyphManager&) = delete;
		GlyphManager& operator = (const GlyphManager&) = delete;
		~GlyphManager() = default;

		static void Create();
		static void Destroy();
		static void Add(Glyph* pGlyph, Glyph::Name name);
		static Glyph* Find(Glyph::Name name, int findKey);

		static void AddXml(Glyph::Name glyphName, const char* assetName, Texture::Name textName);
		

	private:
		GlyphManager();
		static GlyphManager* privGetInstance();
		void privAddToFront(Glyph* pNewData, Glyph*& pHead);
		void privRemove(Glyph* pNewData, Glyph*& pHead);

		Glyph* glyphList;
	};
}
#endif