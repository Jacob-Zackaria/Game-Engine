//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEDDY_MODEL_NEW_H
#define TEDDY_MODEL_NEW_H

#include "Model.h"

namespace Azul
{
	class TeddyModelNew : public Model
	{
	public:
		TeddyModelNew(const char *const modelFileName);
		virtual ~TeddyModelNew();

		// tells the compiler do not create or allow it to be used, c++11
		TeddyModelNew(const TeddyModelNew&) = delete;
		TeddyModelNew&operator=(const TeddyModelNew&other) = delete;

		Matrix*& GetBoneMatrix();

	public:
		void privCreateVAO(const char *const modelFileName) override;

		GLuint vbo_boneweight;   // Bone_weight
		GLuint vbo_boneindex;   // Bone_index

		unsigned int NumBones;
		Matrix *poInversePose;
		Matrix *poBone;
	};
}

#endif

// --- End of File ---
