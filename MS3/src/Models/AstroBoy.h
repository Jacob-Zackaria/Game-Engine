//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ASTRO_BOY_H
#define ASTRO_BOY_H

#include "Model.h"

namespace Azul
{
	class AstroBoy : public Model
	{
	public:
		AstroBoy(const char* const modelFileName);
		~AstroBoy();

		// tells the compiler do not create or allow it to be used, c++11
		AstroBoy(const AstroBoy&) = delete;
		AstroBoy& operator=(const AstroBoy& other) = delete;
	private:
		void privCreateVAO(const char* const modelFileName) override;
	};
}

#endif

// --- End of File ---
