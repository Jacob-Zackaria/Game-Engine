//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ED_PYRAMID_MODEL_H
#define ED_PYRAMID_MODEL_H

#include "Model.h"

namespace Azul
{
	class EdPyramidModel : public Model
	{
	public:
		EdPyramidModel(const char* const modelFileName);
		virtual ~EdPyramidModel();

		// tells the compiler do not create or allow it to be used, c++11
		EdPyramidModel(const EdPyramidModel&) = delete;
		EdPyramidModel& operator=(const EdPyramidModel& other) = delete;

	private:
		void privCreateVAO(const char* const modelFileName) override;

	};
}

#endif

// --- End of File ---