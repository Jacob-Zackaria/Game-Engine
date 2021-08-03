#include "DiamondModel.h"
#include "ModelFileHeader.h"
#include "File.h"

namespace Azul
{
	struct DiamondVert_xyzuvn
	{
		float x;
		float y;
		float z;
		float u;
		float v;
		float nx;
		float ny;
		float nz;
	};
	struct DiamondTri_index
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};

#define WRITE_DATA_TO_FILE 0

#if WRITE_DATA_TO_FILE

#define TRILIST_NUM_TRIANGLES (sizeof(triList)/sizeof(DiamondTri_index))

	DiamondTri_index triList[] =
	{
		{ 0,1,2 }, // front
		{ 0,3,6 }, // right
		{ 0,4,7 }, // back
		{ 0,8,9 }, // left
		{ 10,11,5 }, //bottom front 
		{ 12,13,5 },  //bottom right 
		{ 14,15,5 }, //bottom back 
		{ 16,17,5 }  //bottom left
	};
#define DIAMOND_DATA_NUM_VERTS (sizeof(diamondData)/sizeof(DiamondVert_xyzuvn))

	DiamondVert_xyzuvn  diamondData[] =
	{
		// Triangle 0 // Front
		{ 0.0f,  0.25f,  0.0f, 0.5f, 0.0f, -0.6f,  0.6f, -0.6f },	// v0 top
		{ 0.25f,  -0.25f,  0.25f, 1.0f, 1.0f, -0.6f, -0.6f, -0.6f }, // v1 front right 
		{ -0.25f,  -0.25f,  0.25f, 0.0f, 1.0f,  0.6f,  -0.6f, -0.6f }, // v2 front left
		{ 0.25f,   -0.25f, -0.25f, 1.0f, 1.0f,  0.6f,  0.6f, -0.6f }, // v3 back right
		{ -0.25f, -0.25f,  -0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f }, // v4 back left
		{ 0.0f, -0.75f,  0.0f, 0.5f, 1.0f, 0.6f, -0.6f,  0.6f }, // v5 bottom

		{ 0.25f,  -0.25f,  0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, //v6 (v1) t: 0,1
		{ 0.25f,   -0.25f, -0.25f, 0.0f, 1.0f,  0.6f,  0.6f, -0.6f }, //v7 (v3) t: 0,1
		{ -0.25f,  -0.25f,  0.25f, 1.0f, 1.0f,  0.6f,  -0.6f, -0.6f }, //v8 (v2) t: 1,1
		{ -0.25f, -0.25f,  -0.25f, 0.0f, 1.0f, 0.6f, -0.6f,  0.6f }, //v9 (v4) t:0,1
		{ -0.25f,  -0.25f,  0.25f, 0.0f, 0.0f,  0.6f,  -0.6f, -0.6f }, //v10 (v2) t:0,0
		{ 0.25f,  -0.25f,  0.25f, 1.0f, 0.0f, -0.6f, -0.6f, -0.6f }, //v11 (v1) t:1,0
		{ 0.25f,  -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f, -0.6f }, //v12 (v1) t: 0,0
		{ 0.25f,   -0.25f, -0.25f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f }, //v13 (v3) t: 1,0
		{ 0.25f,   -0.25f, -0.25f, 0.0f, 0.0f,  0.6f,  0.6f, -0.6f }, //v14 (v3) t: 0,0
		{ -0.25f, -0.25f,  -0.25f, 1.0f, 0.0f, 0.6f, -0.6f,  0.6f }, //v15 (v4) t:1,0
		{ -0.25f, -0.25f,  -0.25f, 0.0f, 0.0f, 0.6f, -0.6f,  0.6f }, //v16 (v4) t:0,0
		{ -0.25f,  -0.25f,  0.25f, 1.0f, 0.0f,  0.6f,  -0.6f, -0.6f } //v17 (v2) t:1,0
	};

#endif

	void DiamondModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		ModelFileHeader modelHdr;

#if WRITE_DATA_TO_FILE

		// Fill the ModelFileHdr struct

		this->numVerts = DIAMOND_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		// object name
		strncpy_s(modelHdr.objName, ModelFileHeader::OBJECT_NAME_SIZE, "Diamond", _TRUNCATE);

		// vertex buffer
		modelHdr.numVerts = this->numVerts;
		modelHdr.vertBufferOffset = 0;

		// trilist index 
		modelHdr.numTriList = this->numTris;
		modelHdr.triListBufferOffset = 0;

		// Write to a data file

		ferror = File::Open(fh, pModelFileName, File::Mode::READ_WRITE);
		assert(ferror == File::Error::SUCCESS);

		// write the Header
		ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHeader));
		assert(ferror == File::Error::SUCCESS);

		// write the vert buffer data
		ferror = File::Tell(fh, modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, diamondData, sizeof(DiamondVert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// write the index buffer
		ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, triList, sizeof(DiamondTri_index) * this->numTris);
		assert(ferror == File::Error::SUCCESS);

		// Finish the write
		ferror = File::Flush(fh);
		assert(ferror == File::Error::SUCCESS);

		// Rewind and overwrite model hdr
		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHeader));
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Flush(fh);
		assert(ferror == File::Error::SUCCESS);

		// VERIFY

		ModelFileHeader modelHdr2;

		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, &modelHdr2, sizeof(ModelFileHeader));
		assert(ferror == File::Error::SUCCESS);

		assert(memcmp(&modelHdr, &modelHdr2, sizeof(ModelFileHeader)) == 0);

		// CLOSE

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

#endif

		// READ the data from the file ONLY

		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Read the Hdr
		ferror = File::Read(fh, &modelHdr, sizeof(ModelFileHeader));
		assert(ferror == File::Error::SUCCESS);

		// Using the hdr, allocate the space for the buffers
		this->numVerts = modelHdr.numVerts;
		this->numTris = modelHdr.numTriList;

		// allocate buffers
		DiamondVert_xyzuvn* pDiamondData = new DiamondVert_xyzuvn[(unsigned int)modelHdr.numVerts];
		DiamondTri_index* pTriList = new DiamondTri_index[(unsigned int)modelHdr.numTriList];

		// Read verts
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pDiamondData, sizeof(DiamondVert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// Read trilist
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pTriList, sizeof(DiamondTri_index) * this->numTris);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		// Data is in RAM...
		// Configure and send data to GPU

		// Create a VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);
		glBindVertexArray(this->vao);

		// Create a VBO
		glGenBuffers(1, &this->vbo_verts);
		assert(this->vbo_verts != 0);

		glGenBuffers(1, &this->vbo_trilist);
		assert(this->vbo_trilist != 0);

		// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(DiamondVert_xyzuvn) * this->numVerts), pDiamondData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

			// todo make a table or enum - locations needs enums...

			// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pDiamondData[0].x - (unsigned int)pDiamondData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DiamondVert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pDiamondData[0].u - (unsigned int)pDiamondData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DiamondVert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pDiamondData[0].nx - (unsigned int)pDiamondData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DiamondVert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pDiamondData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(DiamondTri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;
	}

	DiamondModel::DiamondModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	DiamondModel::~DiamondModel()
	{
		// remove anything dynamic here
	}
}