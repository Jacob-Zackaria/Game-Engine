//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TextureManager.h"
#include "Image.h"
#include "Rectangle.h"

namespace Azul
{
	// public methods: -------------------------------------------------------------

	Image::Image()
	{
		this->imageName = Image::Name::Not_Initialized;
		this->imageRect.clear();
		this->pText = 0;
	}

	Image::Image(Image::Name _imageName, Texture *_pText, Rectangle _rect)
	{
		this->imageName = _imageName;
		this->pText = _pText;
		this->imageRect = _rect;
	}

	Image::Image(Image::Name _imageName, Texture::Name _textName, Rectangle _rect)
	{
		this->imageName = _imageName;
		this->pText = TextureManager::Find(_textName);
		this->imageRect = _rect;
	}

	void Image::Set(Image::Name _imageName, Texture *_pText, Rectangle _rect)
	{
		this->imageName = _imageName;
		this->pText = _pText;
		this->imageRect = _rect;
	}

	void Image::Set(Image::Name _imageName, Texture::Name _textName, Rectangle _rect)
	{
		this->imageName = _imageName;
		this->pText = TextureManager::Find(_textName);
		this->imageRect = _rect;
	}
}

//--- End of File ---
