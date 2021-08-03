/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "../Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"

#include "File.h"

// Local function prototypes.
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

static bool gVerbose = true;

// Keenan(3.2.4) - STL
#include "FBX_STL.h"
std::vector<FBX_Vertex> FBX_vert;
std::vector<FBX_Vertex_vsn> FBX_vert_fat;
std::vector<FBX_Trilist> FBX_tri;

// Keenan(3.2.5) - VBO and Reduce
#include "FBX_VBO.h"
std::vector<VBO_Trilist> VBO_tri;
std::vector<FBX_Vertex_vsn> VBO_vert;

std::string TexFileName;
int main(int argc, char** argv)
{
    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;
    bool lResult;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

    // The example can take a FBX file as an argument.
    FbxString lFilePath("");
    /*for (int i = 1, c = argc; i < c; ++i)
    {
        if (FbxString(argv[i]) == "-test") gVerbose = false;
        else if (lFilePath.IsEmpty()) lFilePath = argv[i];
    }*/
    if (argc > 2 && argv[argc - 2] != '\0' && lFilePath.IsEmpty())
    {
        lFilePath = argv[argc - 2];
    }
    else
    {
        printf("\n\nNo Input File!\n\n");
        printf("\n\nExecutable build!\n\n");
        return 0;
    }

    if (lFilePath.IsEmpty())
    {
        lResult = false;
        FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
    }
    else
    {
        FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
        lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
    }

    if (lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");
        FBXSDK_printf("\n\nPlease, provide an input FBX File.\n\n");
        return 0;
    }
    else
    {


        // Keenan(3.2.1) - Triangulate
        FbxGeometryConverter lGeomConverter(lSdkManager);
        // Replace = true
        lGeomConverter.Triangulate(lScene, true);

        FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

        if (gVerbose) DisplayHierarchy(lScene);

        FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

        if (gVerbose) DisplayContent(lScene);

    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);

    // Keenan(3.2.5) - VBO and Reduce

        // Sort ME -----------------------------------------------------

        // copy FBX_vert_fat into FBX_vert_fast_sort
    std::vector<FBX_Vertex_vsn> FBX_vert_fat_sort(FBX_vert_fat);

    // sort it
    // THANK YOU STL SORT!!!!!!
    std::sort(FBX_vert_fat_sort.begin(), FBX_vert_fat_sort.end());

    printf("\n\nFBX SORT\n\n");

    // dump the sort
    FBX_Vertex_vsn vTmp;
    for (int i = 0; i < (int)FBX_vert_fat_sort.size(); i++)
    {
        vTmp = FBX_vert_fat_sort[i];
        printf("FBX_sort[%d]: i:%d v: %f %f %f s: %f %f n:%f %f %f \n",
            i, vTmp.index, vTmp.x, vTmp.y, vTmp.z, vTmp.s, vTmp.t, vTmp.nx, vTmp.ny, vTmp.nz);
    }
    printf("\n\n");

    // REDUCE ME  --------------

    // Destination size (max os the same size of the fat, probably will be smaller)
    VBO_vert.reserve(FBX_vert_fat_sort.size());

    // get the current
    FBX_Vertex_vsn currVert = FBX_vert_fat_sort[0];
    VBO_vert.push_back(currVert);

    for (int i = 0; i < (int)FBX_vert_fat_sort.size(); i++)
    {
        vTmp = FBX_vert_fat_sort[i];

        // if curr != next, then next is good so stort it into redux buffer
        if (!(vTmp == currVert))
        {
            currVert = vTmp;
            VBO_vert.push_back(currVert);
        }
    }

    //// print me - BTW this the final VBO vert buffer
    //for (int i = 0; i < (int)VBO_vert.size(); i++)
    //{
    //    vTmp = VBO_vert[i];
    //printf("VBO_vert[%d]: i:%d v: %f %f %f s: %f %f n:%f %f %f \n",
    //    i, vTmp.index, vTmp.x, vTmp.y, vTmp.z, vTmp.s, vTmp.t, vTmp.nx, vTmp.ny, vTmp.nz);

    //}
    //printf("\n\n");
    //assert(65536 > (int)VBO_vert.size());

    File::Handle vertFile = nullptr, triFile = nullptr, texFile = nullptr;
    File::Error status;
    status = File::Open(vertFile, "Vert.bin", File::Mode::WRITE);
    assert(status == File::Error::SUCCESS);

    status = File::Seek(vertFile, File::Location::BEGIN, 0);
    assert(status == File::Error::SUCCESS);

    FBX_Vertex_vsn* temporary = new FBX_Vertex_vsn();
    for (int i = 0; i < (int)VBO_vert.size(); i++)
    {
        *temporary = VBO_vert.at(i);
        status = File::Write(vertFile, temporary, sizeof(FBX_Vertex_vsn));
        assert(status == File::Error::SUCCESS);
    }
    //// write data.
    //status = File::Write(vertFile, &VBO_vert, sizeof(FBX_Vertex_vsn) * VBO_vert.size());
    //assert(status == File::Error::SUCCESS);
    
    status = File::Flush(vertFile);
    assert(status == File::Error::SUCCESS);

    //close file.
    status = File::Close(vertFile);
    assert(status == File::Error::SUCCESS);

    // now create an update trilist
    VBO_tri.reserve(FBX_tri.size());
    std::vector< FBX_Vertex_vsn >::iterator it;

    int new_v0;
    int new_v1;
    int new_v2;
    VBO_Trilist tmpVBOTri = {};

    for (int i = 0; i < (int)FBX_tri.size(); i++)
    {
        FBX_Trilist old_Tri = FBX_tri[i];
        FBX_Vertex_vsn vert;

        // -- V0 ---------
        // find the old V0 in the fat list unsorted, get the vert
        // goto the new VBO_vert, find the vert get the index

        vert = FBX_vert_fat[old_Tri.v0];

        it = std::find(VBO_vert.begin(), VBO_vert.end(), vert);

        new_v0 = std::distance(VBO_vert.begin(), it);

        // -- V1 ---------

        vert = FBX_vert_fat[old_Tri.v1];

        it = std::find(VBO_vert.begin(), VBO_vert.end(), vert);

        new_v1 = std::distance(VBO_vert.begin(), it);
        // -- V2 ---------

        vert = FBX_vert_fat[old_Tri.v2];

        it = std::find(VBO_vert.begin(), VBO_vert.end(), vert);

        new_v2 = std::distance(VBO_vert.begin(), it);

        // create the VBO_tri

        tmpVBOTri.v0 = (unsigned short)new_v0;
        tmpVBOTri.v1 = (unsigned short)new_v1;
        tmpVBOTri.v2 = (unsigned short)new_v2;

        VBO_tri.push_back(tmpVBOTri);
    }


    //// Done - print VBO trilist 
    //printf("\n\n");
    //for (int i = 0; i < (int)VBO_tri.size(); i++)
    //{
    //    tmpVBOTri = VBO_tri[i];
    //    printf("VBO_tri[%d]: %d %d %d\n", i, tmpVBOTri.v0, tmpVBOTri.v1, tmpVBOTri.v2);
    //}

    status = File::Open(triFile, "Tri.bin", File::Mode::WRITE);
    assert(status == File::Error::SUCCESS);

    status = File::Seek(triFile, File::Location::BEGIN, 0);
    assert(status == File::Error::SUCCESS);

    VBO_Trilist* temporary2 = new VBO_Trilist();
    for (int i = 0; i < (int)VBO_tri.size(); i++)
    {
        *temporary2 = VBO_tri.at(i);
        status = File::Write(triFile, temporary2, sizeof(VBO_Trilist));
        assert(status == File::Error::SUCCESS);
    }

    //// write data.
    //status = File::Write(triFile, &VBO_tri, sizeof(VBO_Trilist) * VBO_tri.size());
    //assert(status == File::Error::SUCCESS);

    status = File::Flush(triFile);
    assert(status == File::Error::SUCCESS);

    //close file.
    status = File::Close(triFile);
    assert(status == File::Error::SUCCESS);


    printf("\n\n %s \n\n", TexFileName.c_str());

    DWORD fileSize = 0;
    // Read Associated Texture.
    status = File::Open(texFile, argv[argc - 3], File::Mode::READ);
    assert(status == File::Error::SUCCESS);

    char* buffer = new char[40 * 1024 * 1024];
       

    status = File::Seek(texFile, File::Location::END, 0);
    assert(status == File::Error::SUCCESS);

    status = File::Tell(texFile, fileSize);
    assert(status == File::Error::SUCCESS);

    status = File::Seek(texFile, File::Location::BEGIN, 0);
    assert(status == File::Error::SUCCESS);

    status = File::Read(texFile, buffer, fileSize);
    assert(status == File::Error::SUCCESS);

    status = File::Close(texFile);
    assert(status == File::Error::SUCCESS);
    //-----
    // write to bin.
    status = File::Open(texFile, "Tex.bin", File::Mode::WRITE);
    assert(status == File::Error::SUCCESS);

    status = File::Seek(texFile, File::Location::BEGIN, 0);
    assert(status == File::Error::SUCCESS);

    status = File::Write(texFile, buffer, fileSize);
    assert(status == File::Error::SUCCESS);

    status = File::Close(texFile);
    assert(status == File::Error::SUCCESS);

    delete[] buffer;

    //--------------------------
    //Read from file.
    // File open.
    vertFile = nullptr;
    status = File::Open(vertFile, "Vert.bin", File::Mode::READ);
    assert(status == File::Error::SUCCESS);

    //create a buffer.
    std::vector<FBX_Vertex_vsn> ttVBO_vert;
    ttVBO_vert.reserve(FBX_vert_fat_sort.size());

    fileSize = 0;
    status = File::Seek(vertFile, File::Location::END, 0);
    assert(status == File::Error::SUCCESS);

    status = File::Tell(vertFile, fileSize);
    assert(status == File::Error::SUCCESS);

    status = File::Seek(vertFile, File::Location::BEGIN, 0);
    assert(status == File::Error::SUCCESS);

    for (int i = 0; i < (int)VBO_vert.size(); i++)
    {
        status = File::Read(vertFile, temporary, sizeof(FBX_Vertex_vsn));
        assert(status == File::Error::SUCCESS);
        ttVBO_vert.push_back(*temporary);
    }

    status = File::Close(vertFile);
    assert(status == File::Error::SUCCESS);

    delete temporary;

    // print me - BTW this the final VBO vert buffer
    for (int i = 0; i < (int)VBO_vert.size(); i++)
    {
        vTmp = ttVBO_vert.at(i);
        printf("VBO_vert[%d]: v: %f %f %f s: %f %f n:%f %f %f \n",
            i, vTmp.x, vTmp.y, vTmp.z, vTmp.s, vTmp.t, vTmp.nx, vTmp.ny, vTmp.nz);

    }
    printf("\n\n");

    //---------------------------
    //---------------------------

    //Read from file.
    // File open.
    triFile = nullptr;
    status = File::Open(triFile, "Tri.bin", File::Mode::READ);
    assert(status == File::Error::SUCCESS);

    //create a buffer.
    std::vector<VBO_Trilist> ttVBO_tri;
    ttVBO_tri.reserve(FBX_tri.size());

    fileSize = 0;
    status = File::Seek(triFile, File::Location::END, 0);
    assert(status == File::Error::SUCCESS);

    status = File::Tell(triFile, fileSize);
    assert(status == File::Error::SUCCESS);

    status = File::Seek(triFile, File::Location::BEGIN, 0);
    assert(status == File::Error::SUCCESS);

    for (int i = 0; i < (int)FBX_tri.size(); i++)
    {
        status = File::Read(triFile, temporary2, sizeof(VBO_Trilist));
        assert(status == File::Error::SUCCESS);
        ttVBO_tri.push_back(*temporary2);
    }

    status = File::Close(triFile);
    assert(status == File::Error::SUCCESS);

    delete temporary2;
    // Done - print VBO trilist 
    printf("\n\n");
    for (int i = 0; i < (int)ttVBO_tri.size(); i++)
    {
        tmpVBOTri = ttVBO_tri.at(i);
        printf("VBO_tri[%d]: %d %d %d\n", i, tmpVBOTri.v0, tmpVBOTri.v1, tmpVBOTri.v2);
    }
    
    printf("\n\n");
    
    ////----------------------------

    // vodka
    system("vodka Vert.bin Vert.blo -t VERTS_TYPE -n ModelVert");
    system("vodka Tri.bin Tri.blo -t POLY_TYPE -n ModelPoly");
    system("vodka Tex.bin zTex.blo -t TEXTURE_TYPE -n ModelTexture");

    system("del *.bin");

    std::string newarg(argv[argc - 1]);
    std::string pckname("Data");
    std::string vername("1.0_rev1");
  
    if (memcmp(argv[1], "-n", 2) == 0 || memcmp(argv[1], "-N", 2) == 0)
    {
        pckname = argv[2];
        if (memcmp(argv[3], "-v", 2) == 0 || memcmp(argv[3], "-V", 2) == 0)
        {
            vername = argv[4];
        }
    }
    else if (memcmp(argv[1], "-v", 2) == 0 || memcmp(argv[1], "-V", 2) == 0)
    {
        vername = argv[2];
        if (memcmp(argv[3], "-n", 2) == 0 || memcmp(argv[3], "-N", 2) == 0)
        {
            pckname = argv[4];
        }
    }
    std::string finalstr("liu " + newarg +" -n " + pckname +" -v " + vername);

    printf("\n\n%s\n\n", finalstr.c_str());

    // liu
    system(finalstr.c_str());

    system("del *.blo");

    printf("\n\n");

    return 0;
}

void DisplayContent(FbxScene* pScene)
{
    int i;
    FbxNode* lNode = pScene->GetRootNode();

    if (lNode)
    {
        for (i = 0; i < lNode->GetChildCount(); i++)
        {
            DisplayContent(lNode->GetChild(i));
        }
    }
}

void DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i;

    if (pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        default:
            break;
        case FbxNodeAttribute::eMarker:
            DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:
            DisplaySkeleton(pNode);
            break;

        case FbxNodeAttribute::eMesh:
            DisplayMesh(pNode);
            break;

        case FbxNodeAttribute::eNurbs:
            DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:
            DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:
            DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:
            DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            DisplayLodGroup(pNode);
            break;
        }
    }

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);

    // Keenan(3.2.2) - Kill the pig
        //DisplayPivotsAndLimits(pNode);

    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for (i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}


void DisplayTarget(FbxNode* pNode)
{
    if (pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char*)pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(FbxNode* pNode)
{
    FBXSDK_printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEulerXYZ:
        FBXSDK_printf("Euler XYZ\n");
        break;
    case eEulerXZY:
        FBXSDK_printf("Euler XZY\n");
        break;
    case eEulerYZX:
        FBXSDK_printf("Euler YZX\n");
        break;
    case eEulerYXZ:
        FBXSDK_printf("Euler YXZ\n");
        break;
    case eEulerZXY:
        FBXSDK_printf("Euler ZXY\n");
        break;
    case eEulerZYX:
        FBXSDK_printf("Euler ZYX\n");
        break;
    case eSphericXYZ:
        FBXSDK_printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case FbxTransform::eInheritRrSs:
        FBXSDK_printf("RrSs\n");
        break;
    case FbxTransform::eInheritRSrs:
        FBXSDK_printf("RSrs\n");
        break;
    case FbxTransform::eInheritRrs:
        FBXSDK_printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(FbxNode* pNode)
{
    FbxVector4 lTmpVector;

    FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case FbxThumbnail::eRGB_24:
                FBXSDK_printf("        Format: RGB\n");
                break;
            case FbxThumbnail::eRGBA_32:
                FBXSDK_printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
            default:
                break;
            case FbxThumbnail::eNotSet:
                FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e64x64:
                FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e128x128:
                FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

