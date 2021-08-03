/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayLink.h"
#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

// -------------------------------------------------
// Keenan(2) Weights
// -------------------------------------------------
#include <string.h>
#include <assert.h>
#include <vector>
#include "BoneWeight.h"

extern std::vector< BoneEntry > vBoneTable;
extern std::vector< PoseInvBoneEntry > vFinalInvPoseTable;

void DisplayLink(FbxGeometry* pGeometry)
{
    //Display cluster now

    //int i, lLinkCount;
    //FbxCluster* lLink;

    int i, j;
    int lSkinCount = 0;
    int lClusterCount = 0;
    FbxCluster* lCluster;

    lSkinCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);

    //-----------
    // Keenan(6)
    //-----------

    for (unsigned int n = 0; n < vBoneTable.size(); n++)
    {
        PoseInvBoneEntry pibe(n, vBoneTable[n].poBoneName);
        vFinalInvPoseTable.push_back(pibe);
    }

    for (int skinIndex = 0; skinIndex < lSkinCount; skinIndex++)
    {
        FbxSkin* skinDeformer = static_cast<FbxSkin*>(pGeometry->GetDeformer(skinIndex, FbxDeformer::eSkin));
        const int clusterCount = skinDeformer->GetClusterCount();

        for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
        {
            FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
            printf("%s \n", cluster->GetLink()->GetName());
            // Get the bone Index
            int boneIndex = -1;
            for (unsigned int n = 0; n < vBoneTable.size(); n++)
            {
                if (strcmp(vBoneTable[n].poBoneName, cluster->GetLink()->GetName()) == 0)
                {
                    boneIndex = n;
                    break;
                }
            }
            assert(boneIndex != -1);

            // Populate invBindPose for each cluster (bone)
            FbxAMatrix linkMatrix;
            FbxAMatrix transformMatrix;
            FbxAMatrix invMatrix;
            cluster->GetTransformMatrix(transformMatrix);
            cluster->GetTransformLinkMatrix(linkMatrix);

            //printf("Keenan: invMatrix [%d] %s\n", boneIndex, cluster->GetLink()->GetName());
            invMatrix = linkMatrix.Inverse() * transformMatrix;


            vFinalInvPoseTable[boneIndex].Set(boneIndex,
                cluster->GetLink()->GetName(),
                invMatrix.GetRow(0),
                invMatrix.GetRow(1),
                invMatrix.GetRow(2),
                invMatrix.GetRow(3));
        }
    }


    // --------

     //lLinkCount = pGeometry->GetLinkCount();
    for (i = 0; i != lSkinCount; ++i)
    {
        lClusterCount = ((FbxSkin*)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
        for (j = 0; j != lClusterCount; ++j)
        {
            DisplayInt("    Cluster ", i);

            lCluster = ((FbxSkin*)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
            //lLink = pGeometry->GetLink(i);    

            const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

            DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

            if (lCluster->GetLink() != NULL)
            {
                DisplayString("        Name: ", (char*)lCluster->GetLink()->GetName());
            }

            FbxString lString1 = "        Link Indices: ";
            FbxString lString2 = "        Weight Values: ";

            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();

            // -------------------------------------------------------------------------
            // Keenan(2) Weights
            // -------------------------------------------------------------------------
            printf("Keenan: bone:%s \n", lCluster->GetLink()->GetName());

            // Find the index given string name
            // YES - this is horrible 
            int boneIndex = -1;
            for (unsigned int n = 0; n < vBoneTable.size(); n++)
            {
                if (strcmp(vBoneTable[n].poBoneName, lCluster->GetLink()->GetName()) == 0)
                {
                    boneIndex = n;
                    break;
                }
            }


            for (k = 0; k < lIndexCount; k++)
            {
                // ----------------------------------------------------
                // Keenan(2) Weights
                // ----------------------------------------------------
                VertWeight vw;
                vw.iVertIndex = lIndices[k];
                vw.fBoneWeight = (float)lWeights[k];

                if (boneIndex != -1)
                {
                    vBoneTable[boneIndex].vVertWeight.push_back(vw);
                }
                else
                {
                    assert(0);
                }


                lString1 += lIndices[k];
                lString2 += (float)lWeights[k];

                if (k < lIndexCount - 1)
                {
                    lString1 += ", ";
                    lString2 += ", ";
                }
            }

            lString1 += "\n";
            lString2 += "\n";

            FBXSDK_printf(lString1);
            FBXSDK_printf(lString2);

            DisplayString("");

            // Keenan(5)

                // Populate invBindPose for each cluster (bone)
            FbxAMatrix linkMatrix;
            FbxAMatrix transformMatrix;
            FbxAMatrix invMatrix;
            lCluster->GetTransformMatrix(transformMatrix);
            lCluster->GetTransformLinkMatrix(linkMatrix);

            printf("Keenan: invMatrix [%d] %s\n", boneIndex, lCluster->GetLink()->GetName());
            invMatrix = linkMatrix.Inverse() * transformMatrix;

            for (k = 0; k < 4; k++)
            {
                FbxVector4 lRow = invMatrix.GetRow(k);
                printf("%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);;
            }
            printf("\n");

            FbxAMatrix lMatrix;

            lMatrix = lCluster->GetTransformMatrix(lMatrix);
            Display3DVector("        Transform Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Scaling: ", lMatrix.GetS());

            lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
            Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

            if (lCluster->GetAssociateModel() != NULL)
            {
                lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
                DisplayString("        Associate Model: ", (char*)lCluster->GetAssociateModel()->GetName());
                Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
                Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
                Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
            }

            DisplayString("");
        }
    }
}

