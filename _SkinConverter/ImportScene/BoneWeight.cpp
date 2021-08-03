
#include <vector>
#include <assert.h>
#include <string.h>

// ------------------------------------------------------
// Keenan(2) Weights
// ------------------------------------------------------

#include "BoneWeight.h"

BoneEntry::BoneEntry(int Index, const char* const pName)
{
	assert(Index >= 0);
	this->iBoneIndex = Index;

	// set everything to 0
	memset(this->poBoneName, 0x0, StringSize);

	// copy n-1 to the buffer
	strncpy_s(this->poBoneName, StringSize, pName, StringSize - 1);

	// Leave this alone... its has no data  
	// this->vVertWeight;

}

void BoneEntry::PrintMe()
{
	printf("Bone[%d] %s \n", this->iBoneIndex, this->poBoneName);
}

// ------------------------------------------------------
// Keenan(6) Pose
// ------------------------------------------------------

PoseInvBoneEntry::PoseInvBoneEntry(int Index,
	const char* const pName)
{
	assert(Index >= 0);
	this->iBoneIndex = Index;

	// set everything to 0
	memset(this->poBoneName, 0x0, StringSize);

	// copy n-1 to the buffer
	strncpy_s(this->poBoneName, StringSize, pName, StringSize - 1);

	// identity
	m.m0 = 1.0f;
	m.m1 = 0.0f;
	m.m2 = 0.0f;
	m.m3 = 0.0f;

	m.m4 = 0.0f;
	m.m5 = 1.0f;
	m.m6 = 0.0f;
	m.m7 = 0.0f;

	m.m8 = 0.0f;
	m.m9 = 0.0f;
	m.m10 = 1.0f;
	m.m11 = 0.0f;

	m.m12 = 0.0f;
	m.m13 = 0.0f;
	m.m14 = 0.0f;
	m.m15 = 1.0f;
}


void PoseInvBoneEntry::Set(int Index,
	const char* const pName,
	FbxVector4 r0,
	FbxVector4 r1,
	FbxVector4 r2,
	FbxVector4 r3)
{
	assert(Index >= 0);
	this->iBoneIndex = Index;

	// set everything to 0
	memset(this->poBoneName, 0x0, StringSize);

	// copy n-1 to the buffer
	strncpy_s(this->poBoneName, StringSize, pName, StringSize - 1);

	m.m0 = (float)r0[0];
	m.m1 = (float)r0[1];
	m.m2 = (float)r0[2];
	m.m3 = (float)r0[3];

	m.m4 = (float)r1[0];
	m.m5 = (float)r1[1];
	m.m6 = (float)r1[2];
	m.m7 = (float)r1[3];

	m.m8 = (float)r2[0];
	m.m9 = (float)r2[1];
	m.m10 = (float)r2[2];
	m.m11 = (float)r2[3];

	m.m12 = (float)r3[0];
	m.m13 = (float)r3[1];
	m.m14 = (float)r3[2];
	m.m15 = (float)r3[3];

}


void PoseInvBoneEntry::PrintMe()
{
	printf("Bone[%d] %s \n", this->iBoneIndex, this->poBoneName);
	printf("   InvPoseMatrix: \n");
	printf("%9.4f %9.4f %9.4f %9.4f\n", m.m0, m.m1, m.m2, m.m3);
	printf("%9.4f %9.4f %9.4f %9.4f\n", m.m4, m.m5, m.m6, m.m7);
	printf("%9.4f %9.4f %9.4f %9.4f\n", m.m8, m.m9, m.m10, m.m11);
	printf("%9.4f %9.4f %9.4f %9.4f\n", m.m12, m.m13, m.m14, m.m15);
	printf("\n");
}
