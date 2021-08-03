#include "PyramidModel.h"
#include "ModelFileHeader.h"
#include "File.h"

namespace Azul
{
	struct Vert_xyzuvn
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
	struct Tri_index
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};

#define WRITE_DATA_TO_FILE 0

#if WRITE_DATA_TO_FILE

#define TRILIST_NUM_TRIANGLES (sizeof(triList)/sizeof(Tri_index))

	Tri_index triList[] =
	{
		{ 0,1,2 }, // front
		{ 0,3,5 }, // right
		{ 0,4,6 }, // back
		{ 0,7,8 }, // left
		{ 9,8,10 }, //bottom left
		{ 8,9,3 }  //bottom right
	};
#define PYRAMID_DATA_NUM_VERTS (sizeof(pyramidData)/sizeof(Vert_xyzuvn))

	Vert_xyzuvn  pyramidData[] =
	{
		// Triangle 0 // Front
		{ 0.0f,  0.25f,  0.0f, 0.5f, 0.0f, -0.6f,  0.6f, -0.6f },	// v0 front top
		{ 0.25f,  -0.25f,  0.25f, 1.0f, 1.0f, -0.6f, -0.6f, -0.6f }, // v1 front right 
		{ -0.25f,  -0.25f,  0.25f, 0.0f, 1.0f,  0.6f,  -0.6f, -0.6f }, // v2 front left

		// Triangle 1 // Right
		{ 0.25f,   -0.25f, -0.25f, 1.0f, 1.0f,  0.6f,  0.6f, -0.6f }, // v3 back right

		// Triangle 2 // Back
		{ -0.25f, -0.25f,  -0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f }, // v4 back left

		{ 0.25f,  -0.25f,  0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, // v5 (v1) t: 0,1
		{ 0.25f,   -0.25f, -0.25f, 0.0f, 1.0f,  0.6f,  0.6f, -0.6f }, //v6 (v3) t: 0,1
		{ -0.25f,  -0.25f,  0.25f, 1.0f, 1.0f,  0.6f,  -0.6f, -0.6f }, //v7 (v2) t: 1,1
		{ -0.25f, -0.25f,  -0.25f, 0.0f, 1.0f, 0.6f, -0.6f,  0.6f }, //v8 (v4) t: 0,1 
		{ 0.25f,  -0.25f,  0.25f, 1.0f, 0.0f, -0.6f, -0.6f, -0.6f }, //v9 (v1) t: 1,0
		{ -0.25f,  -0.25f,  0.25f, 0.0f, 0.0f,  0.6f,  -0.6f, -0.6f }, //v10 (v2) t: 0,0
	};

#endif

	void PyramidModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		ModelFileHeader modelHdr;

#if WRITE_DATA_TO_FILE

		// Fill the ModelFileHdr struct

		this->numVerts = PYRAMID_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		// object name
		strncpy_s(modelHdr.objName, ModelFileHeader::OBJECT_NAME_SIZE, "Pyramid", _TRUNCATE);

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

		ferror = File::Write(fh, pyramidData, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// write the index buffer
		ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, triList, sizeof(Tri_index) * this->numTris);
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
		Vert_xyzuvn* pPyramidData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
		Tri_index* pTriList = new Tri_index[(unsigned int)modelHdr.numTriList];

		// Read verts
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pPyramidData, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// Read trilist
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pTriList, sizeof(Tri_index) * this->numTris);
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
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pPyramidData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

			// todo make a table or enum - locations needs enums...

			// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pPyramidData[0].x - (unsigned int)pPyramidData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pPyramidData[0].u - (unsigned int)pPyramidData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pPyramidData[0].nx - (unsigned int)pPyramidData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pPyramidData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;
	}

	PyramidModel::PyramidModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	PyramidModel::~PyramidModel()
	{
		// remove anything dynamic here
	}
}