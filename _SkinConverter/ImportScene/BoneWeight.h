#ifndef BONE_WEIGHT_H
#define BONE_WEIGHT_H

// -----------------------------------------
// Keenan(2) Weights
// -----------------------------------------

#include <vector>

struct VertWeight
{
	VertWeight()
	{
		iVertIndex = 0;
		fBoneWeight = 0.0f;
	}
	int   iVertIndex;
	float fBoneWeight;
};

struct BoneWeight
{
	BoneWeight()
	{
		iBoneIndex = 0;
		fBoneWeight = 0.0f;
	}
	int   iBoneIndex;
	float fBoneWeight;
};

struct Bone_weightAzul
{
	float a;
	float b;
	float c;
	float d;
};

struct Bone_indexAzul
{
	unsigned int ia;
	unsigned int ib;
	unsigned int ic;
	unsigned int id;
};

struct FinalVertEntry
{
	BoneWeight A;
	BoneWeight B;
	BoneWeight C;
	BoneWeight D;
};

struct VertEntry
{
public:
	std::vector< BoneWeight >  vBoneWeight;
};

struct BoneEntry
{
public:
	static const unsigned int StringSize = 128;

public:
	BoneEntry() = delete;
	BoneEntry(int Index, const char* const pName);

	void PrintMe();

public:
	int iBoneIndex;
	char poBoneName[StringSize];
	std::vector< VertWeight >  vVertWeight;
};

// -----------------------------------------
// Keenan(6) Pose
// -----------------------------------------

#include "fbxsdk.h"

struct PoseInvBoneEntry
{
public:
	static const unsigned int StringSize = 128;

	struct Mat
	{
		float m0;
		float m1;
		float m2;
		float m3;
		float m4;
		float m5;
		float m6;
		float m7;
		float m8;
		float m9;
		float m10;
		float m11;
		float m12;
		float m13;
		float m14;
		float m15;
	};

public:
	PoseInvBoneEntry(int Index,
		const char* const pName);

	void Set(int Index,
		const char* const pName,
		FbxVector4 r0,
		FbxVector4 r1,
		FbxVector4 r2,
		FbxVector4 r3);

	void PrintMe();

public:
	int iBoneIndex;
	char poBoneName[StringSize];
	Mat m;
};

#endif 

// --- End of File ---