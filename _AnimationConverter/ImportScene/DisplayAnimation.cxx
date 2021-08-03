/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/
#include <assert.h>

#include "DisplayCommon.h"
#include "DisplayAnimation.h"

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

#include "File.h"
//File type.
File::Handle fh2 = nullptr;
File::Error status2;

struct Vect
{
	Vect(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z)
	{
	}

	float x;
	float y;
	float z;

};
extern std::string ClipFile;

// Keenan(10)
static int indexChannels = 0;

// Keenan(11)
FbxAnimEvaluator* pAnimEvaluator;

// Keenan(8) - add FbxTime lkeyTime to display
void DisplayAnimationKeenan(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher, FbxTime lKeyTime);
void DisplayChannelsKeenan(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve) (FbxAnimCurve* pCurve), void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher, FbxTime lKeyTime);

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher = false);

void DisplayCurveKeys(FbxAnimCurve* pCurve);
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

void DisplayAnimation(FbxScene* pScene)
{
	//int i = 0;

	// Keenan(11)
	pAnimEvaluator = pScene->GetAnimationEvaluator();

	
	//open file
	status2 = File::Open(fh2, ClipFile.c_str(), File::Mode::WRITE);
	assert(status2 == File::Error::SUCCESS);

	status2 = File::Seek(fh2, File::Location::BEGIN, 0);
	assert(status2 == File::Error::SUCCESS);


	for (int i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

		// Keenan(11) RAM - RULES good find  <--- He read the manual
		pScene->SetCurrentAnimationStack(lAnimStack);

		FbxString lOutputString = "Animation Stack Name: ";
		lOutputString += lAnimStack->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAnimStack, pScene->GetRootNode());
	}

	status2 = File::Flush(fh2);
	assert(status2 == File::Error::SUCCESS);

	//close file.
	status2 = File::Close(fh2);
	assert(status2 == File::Error::SUCCESS);
}

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
	int l;
	int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	int nbAudioLayers = pAnimStack->GetMemberCount<FbxAudioLayer>();
	FbxString lOutputString;

	lOutputString = "   contains ";
	if (nbAnimLayers == 0 && nbAudioLayers == 0)
		lOutputString += "no layers";

	if (nbAnimLayers)
	{
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer";
		if (nbAnimLayers > 1)
			lOutputString += "s";
	}

	if (nbAudioLayers)
	{
		if (nbAnimLayers)
			lOutputString += " and ";

		lOutputString += nbAudioLayers;
		lOutputString += " Audio Layer";
		if (nbAudioLayers > 1)
			lOutputString += "s";
	}
	lOutputString += "\n\n";
	FBXSDK_printf(lOutputString);

	for (l = 0; l < nbAnimLayers; l++)
	{
		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

		if (isSwitcher == false)
		{
			lOutputString = "AnimLayer ";
			lOutputString += l;
			lOutputString += "\n";
			FBXSDK_printf(lOutputString);
		}

		char    lTimeString[256];
		FbxTime   lKeyTime;
		int     lCount = 0;

		// Keenan(10) - GAM575 HACK warning... count is 31 (pay attention)
		for (lCount = 0; lCount < 31; lCount++)
		{

			// Keenan(11) - Note this is hard coded... problem for non-30Hz
			lKeyTime.SetTime(0, 0, 0, lCount, 0, 0, FbxTime::eFrames30);

			if (isSwitcher == false)
			{
				lOutputString = "\n\n ------- Key Time: ";
				lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));

				lOutputString += "  ms: ";
				int msTime = (int)lKeyTime.GetMilliSeconds();
				lOutputString += msTime;
				lOutputString += " ------------------ \n\n";
				printf(lOutputString);
			}

			// Keenan(17) - Skeleton ONLY
			indexChannels = 0;

			DisplayAnimationKeenan(lAnimLayer, pNode, isSwitcher, lKeyTime);
		}
	}

	

	// Keenan(4) - ignoring Audio layers
	for (l = 0; l < nbAudioLayers; l++)
	{
		FbxAudioLayer* lAudioLayer = pAnimStack->GetMember<FbxAudioLayer>(l);

		lOutputString = "AudioLayer ";
		lOutputString += l;
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAudioLayer, isSwitcher);
		FBXSDK_printf("\n");
	}
}

void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool)
{
	int lClipCount;
	FbxString lOutputString;

	lClipCount = pAudioLayer->GetMemberCount<FbxAudio>();

	lOutputString = "     Name: ";
	lOutputString += pAudioLayer->GetName();
	lOutputString += "\n\n";
	lOutputString += "     Nb Audio Clips: ";
	lOutputString += lClipCount;
	lOutputString += "\n";
	FBXSDK_printf(lOutputString);

	for (int i = 0; i < lClipCount; i++)
	{
		FbxAudio* lClip = pAudioLayer->GetMember<FbxAudio>(i);
		lOutputString = "        Clip[";
		lOutputString += i;
		lOutputString += "]:\t";
		lOutputString += lClip->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}

// Keenan(8) - api change
void DisplayAnimationKeenan(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher, FbxTime lKeyTime)
{
	int lModelCount;
	FbxString lOutputString;

	// -------------------------------------------------------------------------
	// Keenan(17) Skeleton ONLY
	// -------------------------------------------------------------------------
	// Thanks Matt Brown - 2021 for the limbNode find
	// -------------------------------------------------------------------------

	// ----- Skeleton ONLY --------------------------------------------------------------------

	FbxNodeAttribute* pAtt = pNode->GetNodeAttribute();
	if (pAtt != nullptr)
	{
		FbxNodeAttribute::EType eType = pAtt->GetAttributeType();

		if (eType == FbxNodeAttribute::EType::eSkeleton)
		{
			FbxSkeleton* lSkeleton = (FbxSkeleton*)pNode->GetNodeAttribute();

			if (lSkeleton && lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
			{
				// Keenan(8)
				if (isSwitcher == false)
				{
					lOutputString = "//   Node Name: ";
					lOutputString += pNode->GetName();
					lOutputString += "\n";
					FBXSDK_printf(lOutputString);
				}
			}
		}
	}

	// Keenan(8) add lKeyTime
	DisplayChannelsKeenan(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher, lKeyTime);
	
	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		// Keenan(8) add lKeyTime
		DisplayAnimationKeenan(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher, lKeyTime);

	}
}

#define MATH_PI_180  0.0174532925f

// Keenan(8) - api change
void DisplayChannelsKeenan(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve) (FbxAnimCurve* pCurve), void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher, FbxTime lKeyTime)
{
	FbxAnimCurve* lAnimCurve = NULL;
	char	buff[256];
	FbxString lOutputString;

	// Keenan(6) Warning Level 4
	FBX_UNUSED(DisplayCurve);
	FBX_UNUSED(DisplayListCurve);
	FBX_UNUSED(pAnimLayer);


	// FbxTime::SetGlobalTimeMode ( FbxTime::eFrames30 );
	// Key to the UNIVERSE:  ........ this function
	// FbxAMatrix  matrix = pNode->EvaluateLocalTransform ( lKeyTime );

	// Keenan(10)
	int index = indexChannels;
	// -------------------------------------------------------------------------
// Keenan(17) Skeleton ONLY
// -------------------------------------------------------------------------
// Thanks Matt Brown - 2021 for the limbNode find
// -------------------------------------------------------------------------

	// ----- Skeleton ONLY --------------------------------------------------------------------

	FbxNodeAttribute* pAtt = pNode->GetNodeAttribute();
	if (pAtt != nullptr)
	{
		FbxNodeAttribute::EType eType = pAtt->GetAttributeType();

		if (eType == FbxNodeAttribute::EType::eSkeleton)
		{
			FbxSkeleton* lSkeleton = (FbxSkeleton*)pNode->GetNodeAttribute();

			if (lSkeleton && lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
			{
				indexChannels++;
				// ----- Skeleton ONLY --------------------------------------------------------------------

							// Keenan(11)
							// skip if there are no curves
				lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				if (!lAnimCurve)
				{
					if (!isSwitcher)
					{
						// lOutputString = "//      ------ No animation data -------- \n";

						FbxAMatrix  matrix = pNode->EvaluateLocalTransform();
						FbxVector4 Trans = matrix.GetT();
						FbxVector4 RotXYZ = matrix.GetR();
						FbxVector4 Scale = matrix.GetS();

						// convert to my format.
						Vect a((float)Trans[0], (float)Trans[1], (float)Trans[2]);
						Vect b((float)(RotXYZ[0] * MATH_PI_180), (float)(RotXYZ[1] * MATH_PI_180), (float)(RotXYZ[2] * MATH_PI_180));
						Vect c((float)Scale[0], (float)Scale[1], (float)Scale[2]);

						// File write.
						status2 = File::Write(fh2, &a, sizeof(Vect));
						assert(status2 == File::Error::SUCCESS);
						// File write.
						status2 = File::Write(fh2, &b, sizeof(Vect));
						assert(status2 == File::Error::SUCCESS);
						// File write.
						status2 = File::Write(fh2, &c, sizeof(Vect));
						assert(status2 == File::Error::SUCCESS);

						lOutputString = "pTmp->poBone[";
						lOutputString += index;
						lOutputString += "].T = Vect( ";
						sprintf(buff, "%ff", Trans[0]);
						lOutputString += buff;
						lOutputString += ", ";
						sprintf(buff, "%ff", Trans[1]);
						lOutputString += buff;
						lOutputString += ", ";
						sprintf(buff, "%ff", Trans[2]);
						lOutputString += buff;
						lOutputString += " );\n";
						printf(lOutputString);

						lOutputString = "pTmp->poBone[";
						lOutputString += index;
						lOutputString += "].Q = Quat( Quat::Rot3::XYZ,";
						sprintf(buff, "%ff", RotXYZ[0] * MATH_PI_180);
						lOutputString += buff;
						lOutputString += ", ";
						sprintf(buff, "%ff", RotXYZ[1] * MATH_PI_180);
						lOutputString += buff;
						lOutputString += ", ";
						sprintf(buff, "%ff", RotXYZ[2] * MATH_PI_180);
						lOutputString += buff;
						lOutputString += " );\n";
						printf(lOutputString);

						lOutputString = "pTmp->poBone[";
						lOutputString += index;
						lOutputString += "].S = Vect( ";
						sprintf(buff, "%ff", Scale[0]);
						lOutputString += buff;
						lOutputString += ", ";
						sprintf(buff, "%ff", Scale[1]);
						lOutputString += buff;
						lOutputString += ", ";
						sprintf(buff, "%ff", Scale[2]);
						lOutputString += buff;
						lOutputString += " );\n\n";

						// Goal format
						//pTmp->poBone[0].T = Vect(...);
						//pTmp->poBone[0].Q = Quat(...);
						//pTmp->poBone[0].S = Vect(...);

						printf(lOutputString);

						return;
					}

				}

				// Display general curves.
				if (!isSwitcher)
				{
					// char    lTimeString[256];

					FbxAMatrix  matrix = pNode->EvaluateLocalTransform(lKeyTime);

					// lOutputString = "// Matrix ----- Key Time: ";
					// lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));

					//	int timeMS = (int)lKeyTime.GetMilliSeconds();
					//	lOutputString += " timeMS: ";
					//	lOutputString += timeMS;

					//  lOutputString += "\n\n";
					//  printf (lOutputString);

					FbxVector4 Trans = matrix.GetT();
					FbxVector4 RotXYZ = matrix.GetR();
					FbxVector4 Scale = matrix.GetS();

					// convert to my format.
					Vect a((float)Trans[0], (float)Trans[1], (float)Trans[2]);
					Vect b((float)(RotXYZ[0] * MATH_PI_180), (float)(RotXYZ[1] * MATH_PI_180), (float)(RotXYZ[2] * MATH_PI_180));
					Vect c((float)Scale[0], (float)Scale[1], (float)Scale[2]);

					// File write.
					status2 = File::Write(fh2, &a, sizeof(Vect));
					assert(status2 == File::Error::SUCCESS);
					// File write.
					status2 = File::Write(fh2, &b, sizeof(Vect));
					assert(status2 == File::Error::SUCCESS);
					// File write.
					status2 = File::Write(fh2, &c, sizeof(Vect));
					assert(status2 == File::Error::SUCCESS);

					lOutputString = "pTmp->poBone[";
					lOutputString += index;
					lOutputString += "].T = Vect( ";
					sprintf(buff, "%ff", Trans[0]);
					lOutputString += buff;
					lOutputString += ", ";
					sprintf(buff, "%ff", Trans[1]);
					lOutputString += buff;
					lOutputString += ", ";
					sprintf(buff, "%ff", Trans[2]);
					lOutputString += buff;
					lOutputString += " );\n";
					printf(lOutputString);

					lOutputString = "pTmp->poBone[";
					lOutputString += index;
					lOutputString += "].Q = Quat( Quat::Rot3::XYZ,";
					sprintf(buff, "%ff", RotXYZ[0] * MATH_PI_180);
					lOutputString += buff;
					lOutputString += ", ";
					sprintf(buff, "%ff", RotXYZ[1] * MATH_PI_180);
					lOutputString += buff;
					lOutputString += ", ";
					sprintf(buff, "%ff", RotXYZ[2] * MATH_PI_180);
					lOutputString += buff;
					lOutputString += " );\n";
					printf(lOutputString);

					lOutputString = "pTmp->poBone[";
					lOutputString += index;
					lOutputString += "].S = Vect( ";
					sprintf(buff, "%ff", Scale[0]);
					lOutputString += buff;
					lOutputString += ", ";
					sprintf(buff, "%ff", Scale[1]);
					lOutputString += buff;
					lOutputString += ", ";
					sprintf(buff, "%ff", Scale[2]);
					lOutputString += buff;
					lOutputString += " );\n\n";

					printf(lOutputString);

				} // if
			}
		}
	}
}


static int InterpolationFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant) return 1;
	if ((flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear) return 2;
	if ((flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic) return 3;
	return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard) return 1;
	if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext) return 2;
	return 0;
}

static int TangentmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto) return 1;
	if ((flags & FbxAnimCurveDef::eTangentAutoBreak) == FbxAnimCurveDef::eTangentAutoBreak) return 2;
	if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB) return 3;
	if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser) return 4;
	if ((flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak) return 5;
	if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak) return 6;
	return 0;
}

static int TangentweightFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone) return 1;
	if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight) return 2;
	if ((flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft) return 3;
	return 0;
}

static int TangentVelocityFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone) return 1;
	if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight) return 2;
	if ((flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft) return 3;
	return 0;
}

void DisplayCurveKeys(FbxAnimCurve* pCurve)
{
	static const char* interpolation[] = { "?", "constant", "linear", "cubic" };
	static const char* constantMode[] = { "?", "Standard", "Next" };
	static const char* cubicMode[] = { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
	static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

	FbxTime   lKeyTime;
	float   lKeyValue;
	char    lTimeString[256];
	FbxString lOutputString;
	int     lCount;

	int lKeyCount = pCurve->KeyGetCount();

	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);

	for (lCount = 0; lCount < lKeyCount; lCount++)
	{
		lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
		lKeyTime = pCurve->KeyGetTime(lCount);

		lOutputString = "            Key Time: ";
		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));

		lOutputString += "           In ms: ";
		long long msTime = lKeyTime.GetMilliSeconds();
		lOutputString += (int)msTime;

		lOutputString += ".... Key Value: ";
		lOutputString += lKeyValue;
		lOutputString += " [ ";
		lOutputString += interpolation[InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount))];
		if ((pCurve->KeyGetInterpolation(lCount) & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
		{
			lOutputString += " | ";
			lOutputString += constantMode[ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount))];
		}
		else if ((pCurve->KeyGetInterpolation(lCount) & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
		{
			lOutputString += " | ";
			lOutputString += cubicMode[TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount))];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode())];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode())];
		}
		lOutputString += " ]";
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}

void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
	FbxTime   lKeyTime;
	int     lKeyValue;
	char    lTimeString[256];
	FbxString lListValue;
	FbxString lOutputString;
	int     lCount;

	int lKeyCount = pCurve->KeyGetCount();

	for (lCount = 0; lCount < lKeyCount; lCount++)
	{
		lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
		lKeyTime = pCurve->KeyGetTime(lCount);

		lOutputString = "            Key Time: ";
		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
		lOutputString += ".... Key Value: ";
		lOutputString += lKeyValue;
		lOutputString += " (";
		lOutputString += pProperty->GetEnumValue(lKeyValue);
		lOutputString += ")";

		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}