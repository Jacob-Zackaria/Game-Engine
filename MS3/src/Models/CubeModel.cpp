#include "CubeModel.h"
#include "ModelFileHeader.h"
#include "File.h"
#include "BoundingSphere.h"

namespace Azul
{
	struct CubeVert_xyzuvn
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
	struct CubeTri_index
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};

#define WRITE_DATA_TO_FILE 0

#if WRITE_DATA_TO_FILE

#define TRILIST_NUM_TRIANGLES (sizeof(triList)/sizeof(CubeTri_index))

	CubeTri_index triList[] =
	{
		{ 0,1,2 }, // back bottom
		{ 2,3,0 }, // back top
		{ 2,4,3 }, // right bottom
		{ 4,5,3 }, // right top
		{ 8,6,9 }, // front bottom
		{ 6,16,9 }, // front top 
		{ 10,1,7 }, // left bottom 
		{ 1,0,7 }, // left top 
		{ 11,12,2 }, // bottom right 
		{ 2,1,13 }, // bottom left 
		{ 0,3,14 }, // top right 
		{ 14,15,0 }  // top left
	};
#define CUBE_DATA_NUM_VERTS (sizeof(cubeData)/sizeof(CubeVert_xyzuvn))

	CubeVert_xyzuvn  cubeData[] =
	{

		{ -0.25f,  0.25f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, // v0 back left top
		{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, // v1 back left bottom
		{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f,  0.6f,  -0.6f, -0.6f }, // v2 back right bottom
		{ 0.25f,   0.25f, -0.25f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f }, // v3 back right top	
		{ 0.25f, -0.25f,  0.25f, 0.0f, 1.0f, 0.6f, -0.6f,  0.6f }, // v4 front right bottom t: 0,1	
		{ 0.25f,  0.25f,  0.25f, 0.0f, 0.0f, 0.6f,  0.6f,  0.6f }, // v5 front right top t: 0,0		
		{ -0.25f, -0.25f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f }, // v6 front left bottom t: 0,1	
		{ -0.25f,  0.25f,  0.25f, 1.0f, 0.0f, -0.6f,  0.6f, 0.6f }, // v7 front left top t: 1,0


		{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f }, // v8 (v4) t: 1,1
		{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f, 0.6f,  0.6f,  0.6f }, //v9 (v5) t: 1,0	
		{ -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, -0.6f, -0.6f, 0.6f }, // v10 (v6) t: 1,1
		{ -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, -0.6f, -0.6f, 0.6f }, // v11 (v6) t: 0,0
		{ 0.25f, -0.25f,  0.25f, 1.0f, 0.0f, 0.6f, -0.6f,  0.6f }, // v12 (v4) t: 1,0
		{ -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, -0.6f, -0.6f, 0.6f }, // v13 (v6) t: 0,0
		{ 0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 0.6f,  0.6f,  0.6f }, //v14 (v5) t: 1,1
		{ -0.25f,  0.25f,  0.25f, 0.0f, 1.0f, -0.6f,  0.6f, 0.6f }, //v15 (v7) t: 0,1
		{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f, -0.6f,  0.6f, 0.6f } // v16 (v7) t: 0,0
	};

#endif

	void CubeModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		ModelFileHeader modelHdr;

#if WRITE_DATA_TO_FILE

		// Fill the ModelFileHdr struct

		this->numVerts = CUBE_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		// object name
		strncpy_s(modelHdr.objName, ModelFileHeader::OBJECT_NAME_SIZE, "Cube", _TRUNCATE);

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

		ferror = File::Write(fh, cubeData, sizeof(CubeVert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// write the index buffer
		ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, triList, sizeof(CubeTri_index) * this->numTris);
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
		CubeVert_xyzuvn* pCubeData = new CubeVert_xyzuvn[(unsigned int)modelHdr.numVerts];
		CubeTri_index* pTriList = new CubeTri_index[(unsigned int)modelHdr.numTriList];

		// Read verts
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pCubeData, sizeof(CubeVert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// Read trilist
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pTriList, sizeof(CubeTri_index) * this->numTris);
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

		// Find Bounding Volume - quick hack
		// MOVE this to converter!!!!!
		Vect* pVerts = new Vect[(unsigned int)this->numVerts]();
		for (int i = 0; i < this->numVerts; i++)
		{
			pVerts[i].set(pCubeData[i].x, pCubeData[i].y, pCubeData[i].z);/*
			Trace::out("%d:  %f %f %f\n", i, pVerts[i][x_enum::x], pVerts[i][y_enum::y], pVerts[i][z_enum::z]);*/
		}

		assert(this->poRefSphere);
		this->poRefSphere->RitterSphere(pVerts, this->numVerts);/*
		Trace::out("Cube Ritter: cntr:%f %f %f  rad: %f ", this->poRefSphere->cntr[x_enum::x], this->poRefSphere->cntr[y_enum::y], this->poRefSphere->cntr[z_enum::z], this->poRefSphere->rad);*/

		delete[] pVerts;
		// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(CubeVert_xyzuvn) * this->numVerts), pCubeData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

			// todo make a table or enum - locations needs enums...

			// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pCubeData[0].x - (unsigned int)pCubeData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pCubeData[0].u - (unsigned int)pCubeData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pCubeData[0].nx - (unsigned int)pCubeData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pCubeData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(CubeTri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;
	}

	CubeModel::CubeModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	CubeModel::~CubeModel()
	{
		// remove anything dynamic here
	}
}