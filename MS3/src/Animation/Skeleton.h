//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

#include "GameObjectAnimation.h"
#include "Bone.h"
#include "GameObjectRigid.h"
#include "HierarchyTable.h"

namespace Azul
{
	class Skeleton
	{
	public:
		struct Data
		{
			const Skeleton::Data& operator = (const Skeleton::Data& other)
			{
				strcpy_s(name, other.name);
				index = other.index;
				parentIndex = other.parentIndex;

				return (*this);
			}

			char name[64];
			int  index;
			int  parentIndex;
		};

	public:
		Skeleton(const char* skelFile, const Vect angles, const Vect startLoc = Vect(0.0f, 0.0f, 0.0f));

		Skeleton() = delete;
		Skeleton(const Skeleton&) = delete;
		Skeleton& operator = (const Skeleton&) = delete;
		~Skeleton();

		const int GetNumBones() const;
		GameObjectRigid* GetGameRigid();
		HierarchyTable* GetTable();

		//-----links----
		void setNext(Skeleton* newNext);
		void setPrev(Skeleton* newPrev);
		Skeleton* getNext() const;
		Skeleton* getPrev() const;

	private:
		void privSetAnimationHierarchy(const Vect angles, const Vect startLoc);
		void privLoadSkeleton(const char* skelFile);

	private:
		int  numBones;
		Skeleton::Data* skelHead;
		GameObjectRigid* pGameRigid;
		HierarchyTable* pHierarchyTable;

		// Links-----
		Skeleton* pNext;
		Skeleton* pPrev;
	};
}

#endif

// --- End of File ---