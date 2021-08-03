//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TGA.h"
#include "Texture.h"

namespace Azul
{

	Texture::Texture()
		: name(Texture::Name::NOT_INITIALIZED),
		textureID(0),
		minFilter(GL_LINEAR),
		magFilter(GL_LINEAR),
		horizWrapMode(GL_CLAMP_TO_EDGE),
		vertWrapMode(GL_CLAMP_TO_EDGE),
		rCount(0),
		width(-1),
		height(-1)
	{
		memset(this->assetName, 0, Texture::ASSET_NAME_SIZE);
	}

	void Texture::set(const char* const _assetName,
		Texture::Name _name,
		GLuint _TextureID,
		GLenum _minFilter,
		GLenum _magFilter,
		GLenum _horizWrapMode,
		GLenum _vertWrapMode,
		int _width,
		int _height)
	{
		memset(this->assetName, 0x0, Texture::ASSET_NAME_SIZE);
		memcpy(this->assetName, _assetName, Texture::ASSET_NAME_SIZE - 1);
		this->name = _name;
		this->magFilter = _magFilter;
		this->minFilter = _minFilter;
		this->horizWrapMode = _horizWrapMode;
		this->vertWrapMode = _vertWrapMode;
		this->textureID = _TextureID;
		this->width = _width;
		this->height = _height;
	}

	void Texture::Dump()
	{
		Trace::out("\n%s", this->assetName);
		Trace::out("\nID: %d\nminFilter: %d\nmagFilter: %d\nhorizontalWrap: %d\nverticalWrap: %d\nRef Count: %d\n",
			this->textureID, this->minFilter, this->magFilter, this->horizWrapMode, this->vertWrapMode, this->rCount);
	}

	void Texture::SetMinFilter(unsigned int newMin)
	{
		this->minFilter = newMin;
	}

	void Texture::SetMagFilter(unsigned int newMag)
	{
		this->magFilter = newMag;
	}

	void Texture::SetHorWrap(unsigned int newHor)
	{
		this->horizWrapMode = newHor;
	}

	void Texture::SetVerWrap(unsigned int newVer)
	{
		this->vertWrapMode = newVer;
	}
}


// --- End of File -----

