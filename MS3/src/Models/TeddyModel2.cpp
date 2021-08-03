//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TeddyModel2.h"
#include "File.h"
#include "AnimPackage.h"

namespace Azul
{
	struct Vert_xyzuvnX
	{
		int index;
		float x;
		float y;
		float z;
		float u;
		float v;
		float nx;
		float ny;
		float nz;
	};

	struct Tri_indexNew
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};

	struct Bone_weightNew
	{
		float a;
		float b;
		float c;
		float d;
	};

	struct Bone_indexNew
	{
		unsigned int ia;
		unsigned int ib;
		unsigned int ic;
		unsigned int id;
	};

	struct MatNew
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
	

	Matrix*& TeddyModelNew::GetBoneMatrix()
	{
		return (this->poBone);
	}

	void TeddyModelNew::privCreateVAO(const char *const modelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create header
		AnimPackage animPack = { 0 };

		ferror = File::Open(fh, modelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		// Read the Hdr
		ferror = File::Read(fh, &animPack, sizeof(AnimPackage));
		assert(ferror == File::Error::SUCCESS);

		// Using the hdr, allocate the space for the buffers
		this->numVerts = (int)animPack.numVerts;
		this->numTris = (int)animPack.numTri;
		this->NumBones = animPack.numInvMatrix;

		// allocate buffers
		Vert_xyzuvnX* pVertData = new Vert_xyzuvnX[animPack.numVerts];
		Tri_indexNew* pTriList = new Tri_indexNew[animPack.numTri];
		Bone_weightNew* pBoneWeight = new Bone_weightNew[animPack.numVerts];
		Bone_indexNew* pBoneIndex = new Bone_indexNew[animPack.numVerts];
		MatNew* pMat = new MatNew[animPack.numInvMatrix];

		// Read vert
		ferror = File::Read(fh, pVertData, animPack.numVerts * sizeof(Vert_xyzuvnX));
		assert(ferror == File::Error::SUCCESS);

		// Read tri
		ferror = File::Read(fh, pTriList, animPack.numTri * sizeof(Tri_indexNew));
		assert(ferror == File::Error::SUCCESS);
		
		// Read BoneWeight
		ferror = File::Read(fh, pBoneWeight, animPack.numVerts * sizeof(Bone_weightNew));
		assert(ferror == File::Error::SUCCESS);

		// read BoneINdex
		ferror = File::Read(fh, pBoneIndex, animPack.numVerts * sizeof(Bone_indexNew));
		assert(ferror == File::Error::SUCCESS);
		
		// Read InvMatr
		ferror = File::Read(fh, pMat, animPack.numInvMatrix * sizeof(MatNew));
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);


		this->poInversePose = new Matrix[this->NumBones]();
		this->poBone = new Matrix[this->NumBones]();


		Matrix matIdentity(Matrix::Special::Identity);
		for (unsigned int i = 0; i < this->NumBones; i++)
		{
			this->poInversePose[i] = *(Matrix *)&pMat[i];
			this->poBone[i] = matIdentity;
		}

		delete[] pMat;

		// Create a VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);

		glBindVertexArray(this->vao);  //<---- active

		// Create a VBO
		glGenBuffers(1, &this->vbo_verts);
		assert(this->vbo_verts != 0);

		glGenBuffers(1, &this->vbo_trilist);
		assert(this->vbo_trilist != 0);

		glGenBuffers(1, &this->vbo_boneweight);
		assert(this->vbo_boneweight != 0);

		glGenBuffers(1, &this->vbo_boneindex);
		assert(this->vbo_boneindex != 0);

		// Load VERTS: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvnX) * this->numVerts), pVertData, GL_STATIC_DRAW);

		// Vertext data in location 0
		void *offsetVert = (void *)((unsigned int)&pVertData[0].x - (unsigned int)pVertData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvnX), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data in location 1
		void *offsetTex = (void *)((unsigned int)&pVertData[0].u - (unsigned int)pVertData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvnX), offsetTex);
		glEnableVertexAttribArray(1);

		// Normal data in location 2
		void *offsetNorm = (void *)((unsigned int)&pVertData[0].nx - (unsigned int)pVertData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvnX), offsetNorm);
		glEnableVertexAttribArray(2);

		delete[] pVertData;

		// Load BoneWeight: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_boneweight);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_weightNew) * this->numVerts), pBoneWeight, GL_STATIC_DRAW);

		// BoneWeight data in location 3
		offsetVert = (void *)((unsigned int)&pBoneWeight[0].a - (unsigned int)pBoneWeight);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Bone_weightNew), offsetVert);
		glEnableVertexAttribArray(3);

		delete[] pBoneWeight;

		// Load BoneIndex: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_boneindex);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_indexNew) * this->numVerts), pBoneIndex, GL_STATIC_DRAW);

		// BoneIndex data in location 4
		offsetVert = (void *)((unsigned int)&pBoneIndex[0].ia - (unsigned int)pBoneIndex);
		glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(Bone_indexNew), offsetVert);
		glEnableVertexAttribArray(4);

		delete[] pBoneIndex;

		// Load TriList: ---------------------------------------------------------

		/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_indexNew) * this->numTris),pTriList, GL_STATIC_DRAW);

		delete[] pTriList;
	}

	TeddyModelNew::TeddyModelNew(const char *const modelFileName)
		: Model(),
		vbo_boneweight(0),
		vbo_boneindex(0),
		NumBones(0),
		poInversePose(nullptr),
		poBone(nullptr)
	{
		this->privCreateVAO(modelFileName);
	}

	TeddyModelNew::~TeddyModelNew()
	{
		// remove anything dynamic here
		delete[] this->poInversePose;
		delete[] this->poBone;
	}

}

// --- End of File ---
