#include "CrossModel.h"
#include "ModelFileHeader.h"
#include "File.h"

namespace Azul
{
	struct CrossVert_xyzuvn
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
	struct CrossTri_index
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};

#define WRITE_DATA_TO_FILE 0

#if WRITE_DATA_TO_FILE

#define TRILIST_NUM_TRIANGLES (sizeof(triList)/sizeof(CrossTri_index))

	CrossTri_index triList[] =
	{
		// top
		// face
		{ 0,2,1 }, 
		{ 2,3,1 }, 

		//rest
		{ 24,5,4 }, 
		{ 24,25,5 }, 
		{ 26,7,27 }, 
		{ 26,3,7 },  
		{ 28,29,30 }, 
		{ 29,6,30 },  
		{ 2,31,32 }, 
		{ 2,32,33 }, 

		// horizontal
		//rest
		{ 22,20,16 }, 
		{ 22,16,18 }, 
		{ 19,34,35 }, 
		{ 23,34,19 }, 
		{ 36,17,37 }, 
		{ 17,21,37 }, 
		{ 38,39,40 }, 
		{ 39,41,40 }, 

		// l face
		{ 42,18,17 }, 
		{ 18,43,17 }, 

		// r face
		{ 21,44,45 }, 
		{ 44,22,45 }, 

		// bottom
		//face
		{ 14,12,15 }, 
		{ 12,13,15 },

		//rest
		{ 12,8,9 }, 
		{ 9,13,12 }, 
		{ 46,47,11 }, 
		{ 11,48,46 }, 
		{ 49,50,10 }, 
		{ 10,51,49 }, 
		{ 52,53,54 }, 
		{ 55,52,54 } 
	};
#define CROSS_DATA_NUM_VERTS (sizeof(crossData)/sizeof(CrossVert_xyzuvn))

	CrossVert_xyzuvn  crossData[] =
	{
		{ -0.05f,  0.35f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v0 top front left
		{ 0.05f,  0.35f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v1 top front right
		{ -0.05f,  0.35f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v2 top back left
		{ 0.05f,  0.35f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v3 top back right

		{ -0.05f,  0.05f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f },	// v4 center top front left
		{ 0.05f,  0.05f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f },	   // v5 center top front right
		{ -0.05f,  0.05f,  -0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f },	// v6 center top back left
		{ 0.05f,  0.05f,  -0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f },	// v7 center top back right

		{ -0.05f,  -0.05f,  0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f },	// v8 center bottom front left
		{ 0.05f,  -0.05f,  0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f },	// v9 center bottom front right
		{ -0.05f,  -0.05f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f },	// v10 center bottom back left
		{ 0.05f,  -0.05f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f },	// v11 center bottom back right


		{ -0.05f,  -0.35f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v12 bottom front left
		{ 0.05f,  -0.35f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v13 bottom front right
		{ -0.05f,  -0.35f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v14 bottom back left
		{ 0.05f,  -0.35f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v15 bottom back right

		{ -0.35f,  0.05f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v16 left front top
		{ -0.35f,  -0.05f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v17 left front bottom
		{ -0.35f,  0.05f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v18 left back top
		{ -0.35f,  -0.05f,  -0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v19 left back bottom

		{ 0.35f,  0.05f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v20 right front top
		{ 0.35f,  -0.05f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v21 right front bottom
		{ 0.35f,  0.05f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v22 right back top
		{ 0.35f,  -0.05f,  -0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // v23 right back bottom

		{ -0.05f,  0.35f,  0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v24 (v0) t:0,0
		{ 0.05f,  0.35f,  0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v25 (v1) t:1,0
		{ 0.05f,  0.35f,  0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v26 (v1) t:0,0
		{ 0.05f,  0.05f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v27 (v5) t:0,1
		{ 0.05f,  0.35f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v28 (v3) t:0,0
		{ -0.05f,  0.35f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v29 (v2) t:1,0
		{ 0.05f,  0.05f,  -0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v30 (v7) t:0,1
		{ -0.05f,  0.35f,  0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v31 (v0) t:1,0
		{ -0.05f,  0.05f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v32 (v4) t:1,1
		{ -0.05f,  0.05f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v33 (v6) t: 1,0
		{ 0.35f,  0.05f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v34 (v22) t:0,0
		{ -0.35f,  0.05f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v35 (v18) t:1,0
		{ -0.35f,  -0.05f,  -0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v36 (v19) t:1,0
		{ 0.35f,  -0.05f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v37 (v23) t:0,0
		{ -0.35f,  0.05f,  0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v38 (v16) t:0,0
		{ 0.35f,  0.05f,  0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v39 (v20) t: 1,0
		{ -0.35f,  -0.05f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v40 (v17) t: 0,1
		{ 0.35f,  -0.05f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v41 (v21) t:1,1
		{ -0.35f,  -0.05f,  -0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v42 (v19) t:0,1
		{ -0.35f,  0.05f,  0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v43 (v16) t:1,0
		{ 0.35f,  0.05f,  0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v44 (v20) t:0,0
		{ 0.35f,  -0.05f,  -0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v45 (v23) t:1,1
		{ 0.05f,  -0.35f,  0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v46 (v13) t: 0,1
		{ 0.05f,  -0.05f,  0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v47 (v9) t:0,0
		{ 0.05f,  -0.35f,  -0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v48 (v15) t:1,1
		{ 0.05f,  -0.35f,  -0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v49 (v15) t:0,1
		{ 0.05f,  -0.05f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v50 (v11) t:0,0
		{ -0.05f,  -0.35f,  -0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v51 (v14) t:1,1
		{ -0.05f,  -0.05f,  -0.05f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v52 (v10) t:0,0
		{ -0.05f,  -0.05f,  0.05f, 1.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //v53 (v8) t:1,0
		{ -0.05f,  -0.35f,  0.05f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, //v54 (v12) t:1,1
		{ -0.05f,  -0.35f,  -0.05f, 0.0f, 1.0f, -0.6f,  0.6f, -0.6f } //v55 (v14) t:0,1
	};

#endif

	void CrossModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		ModelFileHeader modelHdr;

#if WRITE_DATA_TO_FILE

		// Fill the ModelFileHdr struct

		this->numVerts = CROSS_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		// object name
		strncpy_s(modelHdr.objName, ModelFileHeader::OBJECT_NAME_SIZE, "Cross", _TRUNCATE);

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

		ferror = File::Write(fh, crossData, sizeof(CrossVert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// write the index buffer
		ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, triList, sizeof(CrossTri_index) * this->numTris);
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
		CrossVert_xyzuvn* pCrossData = new CrossVert_xyzuvn[(unsigned int)modelHdr.numVerts];
		CrossTri_index* pTriList = new CrossTri_index[(unsigned int)modelHdr.numTriList];

		// Read verts
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pCrossData, sizeof(CrossVert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// Read trilist
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pTriList, sizeof(CrossTri_index) * this->numTris);
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
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(CrossVert_xyzuvn) * this->numVerts), pCrossData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

			// todo make a table or enum - locations needs enums...

			// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pCrossData[0].x - (unsigned int)pCrossData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CrossVert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pCrossData[0].u - (unsigned int)pCrossData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CrossVert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pCrossData[0].nx - (unsigned int)pCrossData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CrossVert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pCrossData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(CrossTri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;
	}

	CrossModel::CrossModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	CrossModel::~CrossModel()
	{
		// remove anything dynamic here
	}
}