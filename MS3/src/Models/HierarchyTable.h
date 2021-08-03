//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HIERARCHY_TABLE_H
#define HIERARCHY_TABLE_H

#include "SSBO.h"

namespace Azul
{
	class HierarchyTable
	{
	public:
		HierarchyTable(unsigned int newDepth);
		~HierarchyTable();

		// tells the compiler do not create or allow it to be used, c++11
		HierarchyTable(const HierarchyTable&) = delete;
		HierarchyTable& operator=(const HierarchyTable& other) = delete;
		
		void SetSBO(unsigned int newNumBone, int* hierarachyData);
		SSBO* GetSBO() const;
		const unsigned int GetHierarchyDepth() const;

	public:
		int* tableData;
		unsigned int HeirarchyDepth;
		SSBO* poHierarchy_SBO;
	};
}

#endif

// --- End of File ---
