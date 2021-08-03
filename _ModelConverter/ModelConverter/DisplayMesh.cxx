/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

// Keenan(3.2.3) - Export Verts
#include <assert.h>

#include "DisplayMesh.h"

#include "DisplayMaterial.h"
#include "DisplayTexture.h"
#include "DisplayLink.h"
#include "DisplayShape.h"
#include "DisplayCache.h"

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

#define MAT_HEADER_LENGTH 200

// Keenan(3.2.4) - STL
#include "FBX_STL.h"
extern std::vector<FBX_Vertex> FBX_vert;
extern std::vector<FBX_Vertex_vsn> FBX_vert_fat;
extern std::vector<FBX_Trilist> FBX_tri;


void DisplayControlsPoints(FbxMesh* pMesh);
void DisplayPolygons(FbxMesh* pMesh);
void DisplayMaterialMapping(FbxMesh* pMesh);
void DisplayTextureMapping(FbxMesh* pMesh);
void DisplayTextureNames(FbxProperty& pProperty, FbxString& pConnectionString);
void DisplayMaterialConnections(FbxMesh* pMesh);
void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial,
	char* header, int pMatId, int l);

void DisplayMesh(FbxNode* pNode)
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

	DisplayString("Mesh Name: ", (char*)pNode->GetName());
	DisplayMetaDataConnections(lMesh);
	DisplayControlsPoints(lMesh);
	DisplayPolygons(lMesh);
	DisplayMaterialMapping(lMesh);
	DisplayMaterial(lMesh);
	DisplayTexture(lMesh);

	// Keenan(3.2.2) - Kill the pig
		// DisplayMaterialConnections(lMesh);

	DisplayLink(lMesh);
	DisplayShape(lMesh);

	DisplayCache(lMesh);
}


void DisplayControlsPoints(FbxMesh* pMesh)
{
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	// Keenan(3.2.3) - Export Verts
		//DisplayString("    Control Points");
	printf("\n");
	printf("k0:  Num control pts: %d \n", lControlPointsCount);

	// Keenan(3.2.4) - STL  reserve space
	FBX_vert.reserve(lControlPointsCount);
	int ssize = FBX_vert.capacity();
	printf("FBX_vert.size %d\n", ssize);

	FBX_Vertex  vTmp;

	for (i = 0; i < lControlPointsCount; i++)
	{
		// Keenan(3.2.3) - Export Verts
			//DisplayInt("        Control Point ", i);
			//Display3DVector("            Coordinates: ", lControlPoints[i]);
		printf(" %d: ----------\n", i);
		printf("       [%d] v: %f %f %f\n", i, lControlPoints[i][0],
			lControlPoints[i][1],
			lControlPoints[i][2]);
		// Keenan(3.2.4) - STL
		vTmp.x = (float)lControlPoints[i][0];
		vTmp.y = (float)lControlPoints[i][1];
		vTmp.z = (float)lControlPoints[i][2];

		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				// Keenan(3.2.3) - Export Verts
					//char header[100];
					//FBXSDK_sprintf(header, 100, "            Normal Vector: "); 

				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					// Keenan(3.2.3) - Export Verts
						//Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
					printf("       [%d] n: %f %f %f\n", i, leNormals->GetDirectArray().GetAt(i)[0],
						leNormals->GetDirectArray().GetAt(i)[1],
						leNormals->GetDirectArray().GetAt(i)[2]);

					// Keenan(3.2.4) - STL
					vTmp.nx = (float)(leNormals->GetDirectArray().GetAt(i)[0]);
					vTmp.ny = (float)(leNormals->GetDirectArray().GetAt(i)[1]);
					vTmp.nz = (float)(leNormals->GetDirectArray().GetAt(i)[2]);

					// Keenan(3.2.4) - STL verify
					printf(" x      --> FBX_V[i]: v:%f %f %f  n: %f %f %f\n", vTmp.x, vTmp.y, vTmp.z, vTmp.nx, vTmp.ny, vTmp.nz);
				}
			}
			else
			{

				// Magic number hack
				printf("       [%d] n: %f %f %f\n", i,
					-3456.0f,
					-3456.0f,
					-3456.0f);

				vTmp.nx = -3456.0f;
				vTmp.ny = -3456.0f;
				vTmp.nz = -3456.0f;

				printf(" x      --> FBX_V[i]: v:%f %f %f  n: %f %f %f\n", vTmp.x, vTmp.y, vTmp.z, vTmp.nx, vTmp.ny, vTmp.nz);

			}
		}

		// Keenan(3.2.4) - STL - STL add to FBX_vert
		FBX_vert.push_back(vTmp);
	}

	DisplayString("");


	// Keenan(3.2.4) - STL - STL dump data	
	printf("\n\n\n FBX_VERTEX\n\n");
	for (int k = 0; k < (int)FBX_vert.size(); k++)
	{
		vTmp = FBX_vert[k];
		printf("FBX_V[%d]: v:%f %f %f  n: %f %f %f\n", k, vTmp.x, vTmp.y, vTmp.z, vTmp.nx, vTmp.ny, vTmp.nz);
	}
}


void DisplayPolygons(FbxMesh* pMesh)
{
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	// Keenan(3.2.3) - Export Verts
	   //DisplayString("    Polygons");
	printf("\n");
	printf("k1:  Num Polys: %d \n", lPolygonCount);

	// Keenan(3.2.4) - STL - STL vert_fat, tri, locals, and tri_count
	FBX_vert_fat.reserve(lPolygonCount * 3);
	FBX_tri.reserve(lPolygonCount);

	FBX_Vertex_vsn	vTmp;
	FBX_Trilist		tmpTrilist;

	int trilist_count = 0;


	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		// Keenan(3.2.3) - Export Verts
			//DisplayInt("        Polygon ", i);
		printf("poly[%d]: --------------- \n", i);

		int l;

		for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
		{
			FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					FBXSDK_sprintf(header, 100, "        Assigned to group: ");
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					DisplayInt(header, polyGroupId);
					break;
				}
			default:
				// any other mapping modes don't make sense
				DisplayString("        \"unsupported group assignment\"");
				break;
			}
		}

		int lPolygonSize = pMesh->GetPolygonSize(i);

		// Keenan(3.2.3) - Export Verts
			// assuming size == 3
			// we did the trianglulate so we should be OK
		assert(lPolygonSize == 3);

		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

			// Keenan(3.2.3) - Export Verts
				//Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);
			printf("        index[%d] v: %f %f %f\n", lControlPointIndex,
				lControlPoints[lControlPointIndex][0],
				lControlPoints[lControlPointIndex][1],
				lControlPoints[lControlPointIndex][2]);

			assert(FBX_vert[lControlPointIndex].x == (float)lControlPoints[lControlPointIndex][0]);
			assert(FBX_vert[lControlPointIndex].y == (float)lControlPoints[lControlPointIndex][1]);
			assert(FBX_vert[lControlPointIndex].z == (float)lControlPoints[lControlPointIndex][2]);

			// Use the one from above
			vTmp.x = FBX_vert[lControlPointIndex].x;
			vTmp.y = FBX_vert[lControlPointIndex].y;
			vTmp.z = FBX_vert[lControlPointIndex].z;

			// Magic number hack
			if (FBX_vert[lControlPointIndex].nx != -3456.0f)
			{
				// normal data was in the control points
				vTmp.nx = FBX_vert[lControlPointIndex].nx;
				vTmp.ny = FBX_vert[lControlPointIndex].ny;
				vTmp.nz = FBX_vert[lControlPointIndex].nz;
			}
			else
			{
				//normal data is in polygon info not in the control points
			}

			if (j == 0)
			{
				tmpTrilist.v0 = trilist_count++;
			}
			else if (j == 1)
			{
				tmpTrilist.v1 = trilist_count++;
			}
			else if (j == 2)
			{
				tmpTrilist.v2 = trilist_count++;
			}
			else
			{
				assert(0 == 1);
			}

			for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			{
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
				FBXSDK_sprintf(header, 100, "            Color vertex: ");

				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
				FBXSDK_sprintf(header, 100, "            Texture UV: ");

				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						Display2DVector(header, leUV->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						// Keenan(3.2.3) - Export Verts
							//Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
						printf("        index[%d] uv: %f %f \n", j, leUV->GetDirectArray().GetAt(lTextureUVIndex)[0],
							leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);

						// Keenan(3.2.4) - STL
						vTmp.s = (float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
						vTmp.t = (float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[1];

					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				FBXSDK_sprintf(header, 100, "            Normal: ");

				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						//Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));

						printf("        index[%d] nxnynz: %f %f %f \n", j,
							leNormal->GetDirectArray().GetAt(vertexId)[0],
							leNormal->GetDirectArray().GetAt(vertexId)[1],
							leNormal->GetDirectArray().GetAt(vertexId)[2]);

						vTmp.nx = (float)leNormal->GetDirectArray().GetAt(vertexId)[0];
						vTmp.ny = (float)leNormal->GetDirectArray().GetAt(vertexId)[1];
						vTmp.nz = (float)leNormal->GetDirectArray().GetAt(vertexId)[2];
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));

						printf("        index[%d] nxnynz: %f %f %f \n", j,
							leNormal->GetDirectArray().GetAt(id)[0],
							leNormal->GetDirectArray().GetAt(id)[1],
							leNormal->GetDirectArray().GetAt(id)[2]);

						vTmp.nx = (float)leNormal->GetDirectArray().GetAt(id)[0];
						vTmp.ny = (float)leNormal->GetDirectArray().GetAt(id)[1];
						vTmp.nz = (float)leNormal->GetDirectArray().GetAt(id)[2];
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
				FBXSDK_sprintf(header, 100, "            Tangent: ");

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

				FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			vertexId++;

			// Keenan(3.2.4) - STL - push vert_fat
			vTmp.index = lControlPointIndex;
			printf("FBX_fat[%d]: v: %f %f %f s: %f %f n:%f %f %f \n",
				vTmp.index,
				vTmp.x,
				vTmp.y,
				vTmp.z,
				vTmp.s,
				vTmp.t,
				vTmp.nx,
				vTmp.ny,
				vTmp.nz);
			FBX_vert_fat.push_back(vTmp);

		} // for polygonSize

		// Keenan(3.2.4) - STL- push tri
		FBX_tri.push_back(tmpTrilist);
		printf("FBX_tri: %d %d %d \n", tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);

	} // for polygonCount


	//check visibility for the edges of the mesh
	for (int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
	{
		FbxGeometryElementVisibility* leVisibility = pMesh->GetElementVisibility(l);
		FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
		DisplayString(header);
		switch (leVisibility->GetMappingMode())
		{
		default:
			break;
			//should be eByEdge
		case FbxGeometryElement::eByEdge:
			//should be eDirect
			for (j = 0; j != pMesh->GetMeshEdgeCount(); ++j)
			{
				DisplayInt("        Edge ", j);
				DisplayBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
			}

			break;
		}
	}

	// Keenan(3.2.3) - Export Verts
	DisplayString("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

	// Keenan(3.2.4) - STL - vert_fat dump
	printf("\n\n\nFBX_VERT_FAT\n\n");
	for (int k = 0; k < (int)FBX_vert_fat.size(); k++)
	{
		vTmp = FBX_vert_fat[k];
		printf("FBX_fat[%d]: i:%d v: %f %f %f s: %f %f n:%f %f %f \n", k, vTmp.index, vTmp.x, vTmp.y, vTmp.z, vTmp.s, vTmp.t, vTmp.nx, vTmp.ny, vTmp.nz);
	}

	// Keenan(3.2.4) - STL - tri dump
	printf("\n\n\nFBX_TRILIST\n\n");
	for (int s = 0; s < (int)FBX_tri.size(); s++)
	{
		tmpTrilist = FBX_tri[s];
		printf("FBX_tri[%d]: %d %d %d \n", s, tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);
	}

}

void DisplayTextureNames(FbxProperty& pProperty, FbxString& pConnectionString)
{
	int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
	if (lLayeredTextureCount > 0)
	{
		for (int j = 0; j < lLayeredTextureCount; ++j)
		{
			FbxLayeredTexture* lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
			pConnectionString += " Texture ";

			for (int k = 0; k < lNbTextures; ++k)
			{
				//lConnectionString += k;
				pConnectionString += "\"";
				pConnectionString += (char*)lLayeredTexture->GetName();
				pConnectionString += "\"";
				pConnectionString += " ";
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " on layer ";
			pConnectionString += j;
		}
		pConnectionString += " |";
	}
	else
	{
		//no layered texture simply get on the property
		int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

		if (lNbTextures > 0)
		{
			pConnectionString += " Texture ";
			pConnectionString += " ";

			for (int j = 0; j < lNbTextures; ++j)
			{
				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
				if (lTexture)
				{
					pConnectionString += "\"";
					pConnectionString += (char*)lTexture->GetName();
					pConnectionString += "\"";
					pConnectionString += " ";
				}
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " |";
		}
	}
}

void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char* header, int pMatId, int l)
{
	if (!pMaterial)
		return;

	FbxString lConnectionString = "            Material %d -- ";
	//Show all the textures

	FbxProperty lProperty;
	//Diffuse Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	DisplayTextureNames(lProperty, lConnectionString);

	//DiffuseFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Emissive Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
	DisplayTextureNames(lProperty, lConnectionString);

	//EmissiveFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
	DisplayTextureNames(lProperty, lConnectionString);


	//Ambient Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
	DisplayTextureNames(lProperty, lConnectionString);

	//AmbientFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Specular Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	DisplayTextureNames(lProperty, lConnectionString);

	//SpecularFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Shininess Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	DisplayTextureNames(lProperty, lConnectionString);

	//Bump Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
	DisplayTextureNames(lProperty, lConnectionString);

	//Normal Map Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	DisplayTextureNames(lProperty, lConnectionString);

	//Transparent Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
	DisplayTextureNames(lProperty, lConnectionString);

	//TransparencyFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Reflection Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
	DisplayTextureNames(lProperty, lConnectionString);

	//ReflectionFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Update header with material info
	bool lStringOverflow = (lConnectionString.GetLen() + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
	if (lStringOverflow)
	{
		// Truncate string!
		lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
		lConnectionString = lConnectionString + "...";
	}
	FBXSDK_sprintf(header, MAT_HEADER_LENGTH, lConnectionString.Buffer(), pMatId, l);
	DisplayString(header);
}

void DisplayMaterialConnections(FbxMesh* pMesh)
{
	int i, l, lPolygonCount = pMesh->GetPolygonCount();

	char header[MAT_HEADER_LENGTH];

	DisplayString("    Polygons Material Connections");

	//check whether the material maps with only one mesh
	bool lIsAllSame = true;
	for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{

		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	//For eAllSame mapping type, just out the material and texture mapping info once
	if (lIsAllSame)
	{
		for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{

			FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
			if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				if (lMatId >= 0)
				{
					DisplayInt("        All polygons share the same material in mesh ", l);
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}

		//no material
		if (l == 0)
			DisplayString("        no material applied");
	}

	//For eByPolygon mapping type, just out the material and texture mapping info once
	else
	{
		for (i = 0; i < lPolygonCount; i++)
		{
			DisplayInt("        Polygon ", i);

			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{

				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
				FbxSurfaceMaterial* lMaterial = NULL;
				int lMatId = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
				lMatId = lMaterialElement->GetIndexArray().GetAt(i);

				if (lMatId >= 0)
				{
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}
	}
}


void DisplayMaterialMapping(FbxMesh* pMesh)
{
	const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct" };

	int lMtrlCount = 0;
	FbxNode* lNode = NULL;
	if (pMesh) {
		lNode = pMesh->GetNode();
		if (lNode)
			lMtrlCount = lNode->GetMaterialCount();
	}

	for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{
		FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial(l);
		if (leMat)
		{
			char header[100];
			FBXSDK_sprintf(header, 100, "    Material Element %d: ", l);
			DisplayString(header);


			DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
			DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);

			int lMaterialCount = 0;
			FbxString lString;

			if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lMaterialCount = lMtrlCount;
			}

			if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int i;

				lString = "           Indices: ";

				int lIndexArrayCount = leMat->GetIndexArray().GetCount();
				for (i = 0; i < lIndexArrayCount; i++)
				{
					lString += leMat->GetIndexArray().GetAt(i);

					if (i < lIndexArrayCount - 1)
					{
						lString += ", ";
					}
				}

				lString += "\n";

				FBXSDK_printf(lString);
			}
		}
	}

	DisplayString("");
}
