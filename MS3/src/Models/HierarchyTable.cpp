
#include "HierarchyTable.h"

namespace Azul
{
	HierarchyTable::HierarchyTable(unsigned int newDepth)
		:
		tableData(nullptr),
		HeirarchyDepth(newDepth)
	{
		this->poHierarchy_SBO = new SSBO();
	}
	HierarchyTable::~HierarchyTable()
	{
		delete this->poHierarchy_SBO;
	}
	void HierarchyTable::SetSBO(unsigned int newNumBone, int* hierarachyData)
	{
		this->poHierarchy_SBO->Set((newNumBone * this->HeirarchyDepth), sizeof(int), &hierarachyData[0]);
	}
	const unsigned int HierarchyTable::GetHierarchyDepth() const
	{
		return this->HeirarchyDepth;
	}
	SSBO* HierarchyTable::GetSBO() const
	{
		return this->poHierarchy_SBO;
	}
}