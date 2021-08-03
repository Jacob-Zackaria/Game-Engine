
#include "AnimationTransition.h"
#include "Clip.h"
#include "Mixer.h"
#include "TeddyModel.h"
#include "ShaderManager.h"
#include "MathApp.h"

namespace Azul
{
	AnimationTransition::AnimationTransition(HierarchyTable* pNewTableData, Clip* pClipA, Clip* pClipB, float blendTime)
		: pSSBO_A(nullptr),
		pSSBO_B(nullptr),
		poSSBO_Result(nullptr),
		pTableData(pNewTableData),
		poBoneWorld_SBO(nullptr),
		numBones(-1),
		tS(blendTime),
		StartEnable(false),
		delta(0.0f),
		pName(AnimationTransition::Name::UNDEFINED),
		pNext(nullptr),
		pPrev(nullptr)
	{
		assert(pClipA);
		assert(pClipB);

		this->pSSBO_A = pClipA->pSSBO_Result;
		this->pSSBO_B = pClipB->pSSBO_Result;

		assert(this->pSSBO_A);
		assert(this->pSSBO_A);

		assert(pClipA->GetNumOfBones() == pClipB->GetNumOfBones());

		this->numBones = pClipA->GetNumOfBones();

		this->poSSBO_Result = new SSBO();
		Bone* pBone = new Bone[(unsigned int)numBones]();
		this->poSSBO_Result->Set((unsigned int)numBones, sizeof(Bone), pBone);
		delete[] pBone;

		this->poBoneWorld_SBO = new SSBO();		
		Matrix *pResultMatrix = new Matrix[(unsigned int)numBones]();
		this->poBoneWorld_SBO->Set((unsigned int)numBones, sizeof(Matrix), pResultMatrix);
		delete[] pResultMatrix;
	}

	AnimationTransition::~AnimationTransition()
	{
		delete this->poSSBO_Result;
		delete this->poBoneWorld_SBO;
	}

	void AnimationTransition::StartTransition()
	{
		this->StartEnable = true;
	}

	void AnimationTransition::privTransitionTime()
	{
		if (this->StartEnable)
		{
			float b = 1.0f;
			float a = 0.0f;
			static float t = 0.0f;
			t += this->delta;
			if (t > 1.0f)
			{
				t = 1.0f;
			}

			if (t < 0.0f)
			{
				t = 0.0f;
			}

			this->tS = a + t * (b - a);
		}
	}

	void AnimationTransition::SetTime(float newTime)
	{
		this->tS = newTime;
	}

	void AnimationTransition::Update()
	{
		this->privTransitionTime();
		Mixer::Blend_GPU(this->pSSBO_A, this->pSSBO_B, this->poSSBO_Result, tS, this->numBones);

		// ------------------------------------------------
		// Compute local to world shader - move this
		// ------------------------------------------------

		// Lock and load the shader...
		ShaderObject* pComputeShader = ShaderManager::Find(ShaderObject::Name::COMPUTE_WORLD);

		pComputeShader->SetActive();

		// need a clean way to store and hold this data...
		SSBO* pBoneLocal = this->poSSBO_Result;
		SSBO* pHierarchyTable = this->pTableData->GetSBO();
		unsigned int hierarchyDepth = this->pTableData->GetHierarchyDepth();

		pBoneLocal->Bind(0);
		pHierarchyTable->Bind(1);
		this->poBoneWorld_SBO->Bind(2);

		glUniform1i(pComputeShader->GetLocation("tableDepth"), (int)hierarchyDepth);
		glUniform1i(pComputeShader->GetLocation("boneCount"), (int)numBones);

		// Dispatch
		pComputeShader->Dispatch(4, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	//-----links----

	void AnimationTransition::setNext(AnimationTransition* newNext)
	{
		this->pNext = newNext;
	}

	void AnimationTransition::setPrev(AnimationTransition* newPrev)
	{
		this->pPrev = newPrev;
	}

	AnimationTransition* AnimationTransition::getNext() const
	{
		return (this->pNext);
	}

	AnimationTransition* AnimationTransition::getPrev() const
	{
		return (this->pPrev);
	}

	AnimationTransition::Name AnimationTransition::getName() const
	{
		return this->pName;
	}

	void AnimationTransition::setName(AnimationTransition::Name inName)
	{
		this->pName = inName;
	}
	void AnimationTransition::SetDelta(float newDelta)
	{
		this->delta = newDelta;
	}
}