//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <string.h>
#include "ModelFileHeader.h"

namespace Azul
{
	ModelFileHeader::ModelFileHeader()
	{
		memset(this, 0x0, sizeof(ModelFileHeader));
	}

}
// --- End of File ---
