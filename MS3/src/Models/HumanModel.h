//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HUMAN_MODEL_H
#define HUMAN_MODEL_H

#include "Model.h"
#include "SSBO.h"
namespace Azul
{
	class HumanModel : public Model
	{
	public:
		HumanModel(const char* const modelFileName);
		virtual ~HumanModel();

		// tells the compiler do not create or allow it to be used, c++11
		HumanModel(const HumanModel&) = delete;
		HumanModel& operator=(const HumanModel& other) = delete;

	public:
		void privCreateVAO(const char* const modelFileName) override;

		GLuint vbo_boneweight;   // Bone_weight
		GLuint vbo_boneindex;   // Bone_index

		unsigned int NumBones;
		SSBO* poInversePose_SBO;
	};
}

#endif

// --- End of File ---
