/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>
#include <assert.h>
#include <vector>

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

#include "File.h"
//File type.
File::Handle fh = nullptr;
File::Error status;

// Hierrachy table
int temp[200] = { -1 };

struct Data_m
{
	Data_m(char* _name,int _index, int _parent)
		:
		index(_index),
		parentIndex(_parent)
	{
		strcpy(name, _name);
	}

	char name[64];
	int  index;
	int  parentIndex;
};
extern std::string SkelFile;

// Keenan(15)
#include <map>
struct BoneName
{
	char name[64];

	BoneName(char* pBuff)
	{
		memset(name, 0, 64);
		memcpy(name, pBuff, strlen(pBuff));
	}

	bool operator == (const BoneName& other) const
	{
		return (strcmp(this->name, other.name) == 0);
	}

	bool operator < (const BoneName& other) const
	{
		return (strcmp(this->name, other.name) < 0);
	}
};
std::map<BoneName, int> BoneNames;


static int displayHierarchyIndex = 0;

// Local functions prototype.
void DisplayHierarchy(FbxNode* pNode, int pDepth);
void DisplayHierarchy(FbxNode* pParent, FbxNode* pNode, int pDepth);

void DisplayHierarchy(FbxScene* pScene)
{
	int i;
	FbxNode* lRootNode = pScene->GetRootNode();


	//open file
	status = File::Open(fh, SkelFile.c_str(), File::Mode::WRITE);
	assert(status == File::Error::SUCCESS);

	status = File::Seek(fh, File::Location::BEGIN, sizeof(int));
	assert(status == File::Error::SUCCESS);


	for (i = 0; i < lRootNode->GetChildCount(); i++)
	{
		// Keenan(7)
		DisplayHierarchy(lRootNode, lRootNode->GetChild(i), 0);
	}

	status = File::Seek(fh, File::Location::BEGIN, 0);
	assert(status == File::Error::SUCCESS);

	// File write hierarchy index.
	status = File::Write(fh, &displayHierarchyIndex, sizeof(int));
	assert(status == File::Error::SUCCESS);

	// write hierarchy table.
	int hierarchyDepth = 0;
	int hierarchyTable[200][200] = { 0 };
	memset(hierarchyTable, -1, sizeof(int) * 200 * 200);

	int maxLoopVal = 0;
	temp[0] = -1;
	hierarchyTable[0][0] = 0;
	hierarchyDepth++;

	for (i = 1; i < displayHierarchyIndex; i++)
	{
		
		if (temp[i] > temp[i - 1])
		{
			if (maxLoopVal == (hierarchyDepth - 1))
			{
				maxLoopVal = hierarchyDepth;
				hierarchyDepth++;
			}
			else
			{
				maxLoopVal++;
			}
		}
		else
		{
			int index;
			for (index = 0; index < maxLoopVal; index++)
			{
				if (hierarchyTable[i - 1][index] == temp[i])
				{
					break;
				}
			}
			maxLoopVal = index + 1;		
		}

		for (int j = 0; j < maxLoopVal; j++)
		{	
			hierarchyTable[i][j] = hierarchyTable[i - 1][j];	
		}
		hierarchyTable[i][maxLoopVal] = i;
	}

	// create final dynamic table.
	const int tableSize = displayHierarchyIndex * hierarchyDepth;
	int* finalTable = new int[(unsigned int)tableSize]();

	// assign values from hierarchy table
	for (i = 0; i < displayHierarchyIndex; i++)
	{
		for (int j = 0; j < hierarchyDepth; j++)
		{
			if (hierarchyTable[i][j] != -1)
			{
				finalTable[(i * hierarchyDepth) + j] = hierarchyTable[i][j];
			}
			else
			{
				finalTable[(i * hierarchyDepth) + j] = displayHierarchyIndex;
			}
		}
	}

	status = File::Seek(fh, File::Location::END, 0);
	assert(status == File::Error::SUCCESS);

	status = File::Write(fh, &hierarchyDepth, sizeof(int));
	assert(status == File::Error::SUCCESS);
	
	
	status = File::Write(fh, (const int*)finalTable, sizeof(int) * tableSize);
	assert(status == File::Error::SUCCESS);
	
	
	//close file.
	status = File::Close(fh);
	assert(status == File::Error::SUCCESS);

	// delete final table
	delete[] finalTable;
}

// Keenan(9) - rework below
void DisplayHierarchy(FbxNode* pParent, FbxNode* pNode, int pDepth)
{
	FbxString lString;

	// Big Hack to remove "Humanoid:" from each name
	char buff[64];
	char buff2[64];
	char origBuff[64];

	// Get original
	// Clean up house cleaning... remove extra stuff on Humanoid
	// Replace spaces with "_" so the names are valid
	strcpy(origBuff, pNode->GetName());

	// test if contains string...
	if (strncmp(origBuff, "Humanoid:", 9) == 0)
	{
		// trim it.
		strcpy(buff, origBuff + 9);
	}
	else
	{
		// leave it
		strcpy(buff, origBuff);
	}

	for (int i = 0; i < (int)strlen(buff); i++)
	{
		// Find a space... replace with an underscore -- quick and dirty
		if (buff[i] == 32)
		{
			buff[i] = 95;
		}
	}

	// Get original
	strcpy(origBuff, pParent->GetName());

	// test if contains string...
	if (strncmp(origBuff, "Humanoid:", 9) == 0)
	{
		// trim it.
		strcpy(buff2, origBuff + 9);
	}
	else
	{
		// leave it
		strcpy(buff2, origBuff);
	}
	for (int i = 0; i < (int)strlen(buff); i++)
	{
		// Find a space... replace with an underscore -- quick and dirty
		if (buff2[i] == 32)
		{
			buff2[i] = 95;
		}
	}


	

	// Add to Map
	BoneName bn(buff);
	BoneNames.emplace(bn, displayHierarchyIndex);

	// See if buff2 is in map
	BoneName bn2(buff2);
	auto it = BoneNames.find(bn2);

	// Can't find it... make -1
	int buff2Index = -1;
	if (it == BoneNames.end())
	{
		buff2Index = -1;
	}
	else
	{
		buff2Index = it->second;
	}

	// -------------------------------------------------------------------------
// Keenan(17) Skeleton ONLY
// -------------------------------------------------------------------------
// Thanks Matt Brown - 2021 for the limbNode find
// -------------------------------------------------------------------------

	FbxNodeAttribute* pAtt = pNode->GetNodeAttribute();
	FbxNodeAttribute::EType eType = pAtt->GetAttributeType();

	if (eType == FbxNodeAttribute::EType::eSkeleton)
	{
		FbxSkeleton* lSkeleton = (FbxSkeleton*)pNode->GetNodeAttribute();

		if (lSkeleton && lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
		{

			lString += "   { ";
			lString += "\"";
			lString += buff;
			lString += "\",";
			lString += " ";
			lString += displayHierarchyIndex;
			lString += ", ";
			lString += buff2Index;
			lString += " },";
			lString += "\n";

			Data_m a(buff, displayHierarchyIndex, buff2Index);

			// File write.
			status = File::Write(fh, &a, sizeof(Data_m));
			assert(status == File::Error::SUCCESS);

			printf(lString.Buffer());

			// tmp table creation.
			temp[displayHierarchyIndex] = buff2Index;

			displayHierarchyIndex++;
			
		}
	}

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayHierarchy(pNode, pNode->GetChild(i), pDepth + 1);
	}
}