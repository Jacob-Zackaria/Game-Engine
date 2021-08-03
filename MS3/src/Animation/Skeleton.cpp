//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "EdPyramidModel.h"
#include "GraphicsObject_TextureLight.h"
#include "GameObjectManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "GraphicsObject_Null.h"
#include "NullModel.h"
#include "Skeleton.h"
#include "PCSTreeForwardIterator.h"
#include "File.h"

namespace Azul
{
	Skeleton::Skeleton(const char* skelFile, const Vect angles, const Vect startLoc)
		: 
		numBones(0),
		skelHead(nullptr),
		pGameRigid(nullptr),
		pHierarchyTable(nullptr),
		pNext(nullptr),
		pPrev(nullptr)
	{
		this->privLoadSkeleton(skelFile);
		this->privSetAnimationHierarchy(angles, startLoc);
	}


	Skeleton::~Skeleton()
	{
		delete[] skelHead;
		delete this->pHierarchyTable;
	}

	const int Skeleton::GetNumBones() const
	{
		return this->numBones;
	}

	GameObjectRigid* Skeleton::GetGameRigid()
	{
		return this->pGameRigid;
	}

	HierarchyTable* Skeleton::GetTable()
	{
		return this->pHierarchyTable;
	}

	void Skeleton::privLoadSkeleton(const char* skelFile)
	{
		File::Handle skelHandle;
		File::Error status;
		status = File::Open(skelHandle, skelFile, File::Mode::READ);
		assert(status == File::Error::SUCCESS);

		status = File::Seek(skelHandle, File::Location::BEGIN, 0);
		assert(status == File::Error::SUCCESS);

		status = File::Read(skelHandle, &this->numBones, sizeof(int));
		assert(status == File::Error::SUCCESS);	

		this->skelHead = new Skeleton::Data[(unsigned int)this->numBones]();

		// read all data.
		for (int j = 0; j < this->numBones; j++)
		{
			Skeleton::Data pTmp;
			status = File::Read(skelHandle, &pTmp, sizeof(Skeleton::Data));
			assert(status == File::Error::SUCCESS);

			this->skelHead[j] = pTmp;
		}

		// read table data.	
		int hierarchyDepth = 0;
		status = File::Read(skelHandle, &hierarchyDepth, sizeof(int));
		assert(status == File::Error::SUCCESS);
		
		// create final dynamic table.
		const int finalTableSize = this->numBones * hierarchyDepth;
		int* finalTable = new int[(unsigned int)finalTableSize]();
				
		status = File::Read(skelHandle, (int*)finalTable, sizeof(int) * finalTableSize);
		assert(status == File::Error::SUCCESS);
				
		this->pHierarchyTable = new HierarchyTable((unsigned int)hierarchyDepth);
		this->pHierarchyTable->SetSBO((unsigned int)this->numBones, finalTable);

		delete[] finalTable;

		status = File::Close(skelHandle);
		assert(status == File::Error::SUCCESS);
	}

	void Skeleton::privSetAnimationHierarchy(const Vect angles, const Vect startLoc)
	{
		// Null object
		NullModel* pNullModel = new NullModel(0);
		ModelManager::Add(pNullModel);
		ShaderObject* pShaderObject_null = new ShaderObject(ShaderObject::Name::NULL_SHADER, "./Shaders/nullRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_null);
		ShaderManager::Add(pShaderObject_null);
		GraphicsObject_Null* pGraphicsObjNull = new GraphicsObject_Null(pNullModel, pShaderObject_null);
		pGameRigid = new GameObjectRigid(pGraphicsObjNull);
		pGameRigid->angle_x = angles[x];
		pGameRigid->angle_y = angles[y];
		pGameRigid->angle_z = angles[z];
		pGameRigid->SetTrans(startLoc);
		pGameRigid->SetName("Rigid");
		GameObjectManager::Add(pGameRigid, GameObjectManager::GetRoot());
	}

	//-----links----

	void Skeleton::setNext(Skeleton* newNext)
	{
		this->pNext = newNext;
	}

	void Skeleton::setPrev(Skeleton* newPrev)
	{
		this->pPrev = newPrev;
	}

	Skeleton* Skeleton::getNext() const
	{
		return (this->pNext);
	}

	Skeleton* Skeleton::getPrev() const
	{
		return (this->pPrev);
	}
}

// --- End of File ---