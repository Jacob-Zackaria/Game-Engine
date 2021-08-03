//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef IMAGE_H
#define IMAGE_H

#include "Rectangle.h"
#include "Texture.h"
#include "ImageLink.h"

namespace Azul
{
	class Image : public ImageLink
	{
	public:
		enum class Name
		{
			Alien_Blue,
			Alien_Green,
			Alien_Red,
			Stitch,
			Not_Initialized,
		};
	public:
		// public methods: -------------------------------------------------------------
		Image();
		Image(Image::Name imageName, Texture::Name textName, Rectangle rect);
		Image(Image::Name imageName, Texture *pTexture, Rectangle rect);

		void Set(Image::Name imageName, Texture::Name textName, Rectangle rect);
		void Set(Image::Name imageName, Texture *pTexture, Rectangle rect);
		//EnumName getName();

	public:
		// data: -----------------------------------------------------------------------
		Image::Name	    imageName;
		Texture *pText;
		Rectangle			imageRect;
	};

}


#endif

//--- End of File ---
