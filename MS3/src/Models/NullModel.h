//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef NULL_MODEL_H
#define NULL_MODEL_H

#include "Model.h"

namespace Azul
{
	class NullModel : public Model
	{
	public:
		NullModel(const char* const modelFileName);
		~NullModel();

		// tells the compiler do not create or allow it to be used, c++11
		NullModel(const NullModel&) = delete;
		NullModel& operator=(const NullModel& other) = delete;

	private:
		virtual void privCreateVAO(const char* const modelFileName) override;


	};

}

#endif

// --- End of File ---
