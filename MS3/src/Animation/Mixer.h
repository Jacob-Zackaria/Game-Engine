//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MIXER_H
#define MIXER_H

#include "AnimTimer.h"
#include "Bone.h"
#include "SSBO.h"
#include "sb7.h"
namespace Azul
{
	class Mixer
	{
	public:
		static void Blend(	Bone *pResult,
							const Bone *pA,
							const Bone *pB,
							const float tS,
							const int numBones);

		static void Blend_GPU(
			SSBO* pKey_A,
			SSBO* PKey_B,
			SSBO* pResult_GPU,
			const float tS,
			const int numBones);
	};
}

#endif

// --- End of File ---
