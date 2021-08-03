//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef WAR_BEAR_H
#define WAR_BEAR_H

#include "Model.h"

namespace Azul
{
	class WarBearModel : public Model
	{
	public:
		WarBearModel(const char* const modelFileName);
		~WarBearModel();

		// tells the compiler do not create or allow it to be used, c++11
		WarBearModel(const WarBearModel&) = delete;
		WarBearModel& operator=(const WarBearModel& other) = delete;
	private:
		void privCreateVAO(const char* const modelFileName) override;
	};
}

#endif

// --- End of File ---
