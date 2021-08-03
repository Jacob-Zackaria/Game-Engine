//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace Azul
{
	struct Rectangle
	{
		Rectangle(float x, float y, float width, float height);
		Rectangle();

		void clear();

		float x;
		float y;
		float width;
		float height;
	};
}

#endif

// --- End of File ---
