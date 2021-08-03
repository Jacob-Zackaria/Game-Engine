#include "MilitaryModel.h"
#include "File.h"
#include "BoundingSphere.h"
#include "PackageHeader.h"
#include "ChunkHeader.h"
#include "TextureManager.h"

namespace Azul
{
	struct MilitaryVert_xyzuvn
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
	struct MilitaryTri_index
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
	};



	void MilitaryModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		PackageHeader pckHdr;
		ChunkHeader chunkHdr;
		// READ the data from the file ONLY

		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		// Read the Hdr
		ferror = File::Read(fh, &pckHdr, sizeof(PackageHeader));
		assert(ferror == File::Error::SUCCESS);

		// Using the hdr, allocate the space for the buffers
		this->numVerts = (int)pckHdr.numVerts;
		this->numTris = (int)pckHdr.numTri;

		// allocate buffers
		MilitaryVert_xyzuvn* pVertData = new MilitaryVert_xyzuvn[pckHdr.numVerts];
		MilitaryTri_index* pTriList = new MilitaryTri_index[pckHdr.numTri];

		// Read chunkhdr
		ferror = File::Read(fh, &chunkHdr, sizeof(ChunkHeader));
		assert(ferror == File::Error::SUCCESS);

		// Read tri
		ferror = File::Read(fh, pTriList, chunkHdr.chunkSize);
		assert(ferror == File::Error::SUCCESS);

		// Read chunkhdr
		ferror = File::Read(fh, &chunkHdr, sizeof(ChunkHeader));
		assert(ferror == File::Error::SUCCESS);

		// Read vert
		ferror = File::Read(fh, pVertData, chunkHdr.chunkSize);
		assert(ferror == File::Error::SUCCESS);

		if (pckHdr.texAvailable == true)
		{
			// Read chunkhdr
			ferror = File::Read(fh, &chunkHdr, sizeof(ChunkHeader));
			assert(ferror == File::Error::SUCCESS);

			char* buffer = new char[40 * 1024 * 1024];
			// Read tex
			ferror = File::Read(fh, buffer, chunkHdr.chunkSize);
			assert(ferror == File::Error::SUCCESS);

			ferror = File::Close(fh);
			assert(ferror == File::Error::SUCCESS);

			TextureManager::AddBuff(buffer, "MilitaryTexture", Texture::Name::MILITARY_TRUCK);

			delete[] buffer;
		}
		else
		{
			ferror = File::Close(fh);
			assert(ferror == File::Error::SUCCESS);

			Trace::out("\n\nNO TEXTURE DATA IN PACKAGE.");
		}



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
		Vect* pVerts = new Vect[(unsigned int)this->numVerts];
		for (int i = 0; i < this->numVerts; i++)
		{
			pVerts[i].set(pVertData[i].x, pVertData[i].y, pVertData[i].z);/*
			Trace::out("%d:  %f %f %f\n", i, pVerts[i][x_enum::x], pVerts[i][y_enum::y], pVerts[i][z_enum::z]);*/
		}

		assert(this->poRefSphere);
		this->poRefSphere->RitterSphere(pVerts, this->numVerts);/*
		Trace::out("Space SHip Ritter: cntr:%f %f %f  rad: %f ", this->poRefSphere->cntr[x_enum::x], this->poRefSphere->cntr[y_enum::y], this->poRefSphere->cntr[z_enum::z], this->poRefSphere->rad);*/

		delete[] pVerts;
		// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(MilitaryVert_xyzuvn) * this->numVerts), pVertData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

			// todo make a table or enum - locations needs enums...

			// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pVertData[0].x - (unsigned int)pVertData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MilitaryVert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pVertData[0].u - (unsigned int)pVertData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MilitaryVert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pVertData[0].nx - (unsigned int)pVertData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MilitaryVert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pVertData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_trilist);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(MilitaryTri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;
	}

	MilitaryModel::MilitaryModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	MilitaryModel::~MilitaryModel()
	{
		// remove anything dynamic here
	}
}