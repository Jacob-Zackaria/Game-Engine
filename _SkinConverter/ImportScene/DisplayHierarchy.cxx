/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

// -----------------------------------------------------------
// Keenan(2) Weights
// -----------------------------------------------------------
			// Bone Weight....
			#include <vector>
			#include "BoneWeight.h"

			extern std::vector< BoneEntry > vBoneTable;
			int BoneCount = 0;



// Local functions prototype.
void DisplayHierarchy(FbxNode* pNode, int pDepth);

void DisplayHierarchy(FbxScene* pScene)
{
    int i;
    FbxNode* lRootNode = pScene->GetRootNode();

    for( i = 0; i < lRootNode->GetChildCount(); i++)
    {
        DisplayHierarchy(lRootNode->GetChild(i), 0);
    }


	// ----------------------------------------------------------------
	// Keenan(2) Weights
	// ----------------------------------------------------------------
			printf("\n\n Bone Table \n");
			for (unsigned int j = 0; j < vBoneTable.size(); j++)
			{
				vBoneTable[j].PrintMe();

				//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[0].iVertIndex, vBoneTable[j].vVertWeight[0].fBoneWeight);
				//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[1].iVertIndex, vBoneTable[j].vVertWeight[1].fBoneWeight);
				//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[2].iVertIndex, vBoneTable[j].vVertWeight[2].fBoneWeight);
				//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[3].iVertIndex, vBoneTable[j].vVertWeight[3].fBoneWeight);
			}

}

void DisplayHierarchy(FbxNode* pNode, int pDepth)
{
	FbxString lString;
	int i;

	// -------------------------------------------------------------------------
	// Keenan(4) Weights
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
			vBoneTable.push_back(BoneEntry(BoneCount, pNode->GetName()));
			BoneCount++;
		}
	}

	for (i = 0; i < pDepth; i++)
	{
		lString += "     ";
	}

	lString += pNode->GetName();
	lString += "\n";

	// -------------------------------------------------------------------------
	// Keenan(2) Weights
	// -------------------------------------------------------------------------
			FBXSDK_printf(lString.Buffer());


	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayHierarchy(pNode->GetChild(i), pDepth + 1);
	}
}



