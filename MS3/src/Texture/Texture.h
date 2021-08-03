//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_H
#define TEXTURE_H

#include "sb7.h"
#include "TextureLink.h"

namespace Azul
{
	typedef int ReferenceCount;
	class Texture : public TextureLink
	{
	public:
		static const unsigned int ASSET_NAME_SIZE = 64;

		enum class Name
		{
			STONES,
			RED_BRICK,
			DUCKWEED,
			ROCKS,
			INVADERS,
			SPACE_SHIP,
			ASTRO_BOY,
			MILITARY_TRUCK,
			TEDDY,
			WAR_BEAR,
			POLAR_BEAR,
			MISSILE,
			CONSOLAS_FONT,
			BRADLEY_FONT,
			DEFAULT,
			NOT_INITIALIZED
		};

	public:
		Texture();

		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;
		
		virtual ~Texture() = default;

		void set(const char* const _assetName,
			Texture::Name _name,
			GLuint _TextureID,
			GLenum minFilter,
			GLenum magFilter,
			GLenum horizWrapMode,
			GLenum vertWrapMode,
			int _width,
			int _height);

		// set min filter
		void SetMinFilter(unsigned int);

		// set mag filter
		void SetMagFilter(unsigned int);

		// set horizontal wrap.
		void SetHorWrap(unsigned int);

		// set vertical wrap.
		void SetVerWrap(unsigned int);

		void Dump();

	private:
		char assetName[Texture::ASSET_NAME_SIZE];

	public:
		Texture::Name name;
		GLuint textureID;
		GLenum minFilter;
		GLenum magFilter;
		GLenum horizWrapMode;
		GLenum vertWrapMode;
		ReferenceCount rCount;
		int width;
		int height;
	};

}
#endif

// --- End of File --------------------------------------------------
