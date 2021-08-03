//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "File.h"
#include "PackageHeader.h"
#include "ChunkHeader.h"
#include "FBX_STL.h"
#include "FBX_VBO.h"
//---------------------------------------------------------------------------
// Leave this signature - do your LIU magic here
// Feel free to add files and methods to this project
//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	if (argc < 6)
	{
		printf("\nError : Fewer arguments.");
		return(-1);
	}
	else if (argc > 6)
	{
		printf("\nError : Extra arguments.");
		return(-1);
	}

	unsigned short int vtype = 0, ntype = 0;
	//check command.
	if (memcmp(argv[2], "-n", 2) == 0 || memcmp(argv[2], "-N", 2) == 0)
	{
		ntype = 3;
		if (memcmp(argv[4], "-v", 2) == 0 || memcmp(argv[4], "-V", 2) == 0)
		{
			vtype = 5;
		}
		else
		{
			printf("\nError: invalid command");
			return(-1);
		}

	}
	else if (memcmp(argv[2], "-v", 2) == 0 || memcmp(argv[2], "-V", 2) == 0)
	{
		vtype = 3;
		if (memcmp(argv[4], "-n", 2) == 0 || memcmp(argv[4], "-N", 2) == 0)
		{
			ntype = 5;
		}
		else
		{
			printf("\nError: invalid command");
			return(-1);
		}
	}
	else
	{
		printf("\nError: invalid command");
		return(-1);
	}

	PackageHeader pckHdr = { 0 };

	// Set package name.
	//Find Length.
	unsigned int i = 0;
	char* traverse = argv[ntype];
	while (traverse[i] != '\0' && i < (PackageHeader::PACKAGE_NAME_SIZE - 1))
	{
		i++;
	}

	if (i >= (PackageHeader::PACKAGE_NAME_SIZE - 1))
	{
		printf("\nLong name not allowed.");
		return(-1);
	}
	else if (i == 0)
	{
		printf("\nError: Unknown name.");
		return(-1);
	}
	memcpy_s(pckHdr.packageName, PackageHeader::PACKAGE_NAME_SIZE, argv[ntype], i + 1);
	
	// Set version string.
	//Find Length.
	i = 0;
	traverse = argv[vtype];
	while (traverse[i] != '\0' && i < (PackageHeader::PACKAGE_VERSION_SIZE - 1))
	{
		i++;
	}

	if (i >= (PackageHeader::PACKAGE_VERSION_SIZE - 1))
	{
		printf("\nLong name not allowed.");
		return(-1);
	}
	else if (i == 0)
	{
		printf("\nError: Unknown name.");
		return(-1);
	}
	memcpy_s(pckHdr.versionString, PackageHeader::PACKAGE_VERSION_SIZE, argv[vtype], i + 1);

	File::Error status;

	// create an array of pointers.
	char** nameArray = new char*[20];

	WIN32_FIND_DATAA fileData = { 0 };
	File::Handle fh = nullptr;
	fh = FindFirstFile("*.blo", &fileData);
	if (fh == INVALID_HANDLE_VALUE)
	{
		assert(false);
	}

	//else copy name from fileData to namearray.
	unsigned int j = 0;
	i = 0;
	while (fileData.cFileName[i] != '\0')
	{
		i++;
	}
	nameArray[j] = new char[20];
	memcpy_s(nameArray[j], i + 1, fileData.cFileName, i + 1);
	j++;

	while (FindNextFile(fh, &fileData) != 0)
	{
		i = 0;
		while (fileData.cFileName[i] != '\0')
		{
			i++;
		}
		nameArray[j] = new char[20];
		memcpy_s(nameArray[j], i + 1, fileData.cFileName, i + 1);
		j++;
	}

	// now we have all .blo files.
	//Read and append to package.
	File::Handle packFile = nullptr;
	File::Handle bloFile = nullptr;

	printf("\n\n%s\n\n", argv[1]);

	status = File::Open(packFile, argv[1], File::Mode::WRITE);
	assert(status == File::Error::SUCCESS);

	status = File::Seek(packFile, File::Location::BEGIN, 0);
	assert(status == File::Error::SUCCESS);

	//create a buffer.
	char* buffer = new char[40 * 1024 * 1024];
	DWORD fileSize = 0;
	ChunkHeader chunkHdr;

	// write header.
	status = File::Write(packFile, &pckHdr, sizeof(PackageHeader));
	assert(status == File::Error::SUCCESS);

	i = 0;
	while (i < j)
	{
		//open blo file.
		status = File::Open(bloFile, nameArray[i], File::Mode::READ);
		assert(status == File::Error::SUCCESS);

		status = File::Seek(bloFile, File::Location::END, 0);
		assert(status == File::Error::SUCCESS);

		status = File::Tell(bloFile, fileSize);
		assert(status == File::Error::SUCCESS);

		status = File::Seek(bloFile, File::Location::BEGIN, 0);
		assert(status == File::Error::SUCCESS);

		// read chunk header.
		status = File::Read(bloFile, &chunkHdr, sizeof(ChunkHeader));
		assert(status == File::Error::SUCCESS);

		pckHdr.totalSize += fileSize;

		if (chunkHdr.type == ChunkType::VERTS_TYPE)
		{
			pckHdr.numVerts = (chunkHdr.chunkSize / sizeof(FBX_Vertex_vsn));
		}
		else if (chunkHdr.type == ChunkType::POLY_TYPE)
		{
			pckHdr.numTri = (chunkHdr.chunkSize / sizeof(VBO_Trilist));
		}
		else if (chunkHdr.type == ChunkType::TEXTURE_TYPE)
		{
			pckHdr.texAvailable = true;
		}

		// read blo file.
		status = File::Read(bloFile, buffer, chunkHdr.chunkSize);
		assert(status == File::Error::SUCCESS);

		//close file.
		status = File::Close(bloFile);
		assert(status == File::Error::SUCCESS);

		// write chunkheader.
		status = File::Write(packFile, &chunkHdr, sizeof(ChunkHeader));
		assert(status == File::Error::SUCCESS);

		// write data.
		status = File::Write(packFile, buffer, chunkHdr.chunkSize);
		assert(status == File::Error::SUCCESS);

		i++;
	}

	pckHdr.numChunks = i;

	// Finish the write
	status = File::Flush(packFile);
	assert(status == File::Error::SUCCESS);

	//overwrite package hdr.
	status = File::Seek(packFile, File::Location::BEGIN, 0);
	assert(status == File::Error::SUCCESS);

	// write data.
	status = File::Write(packFile, &pckHdr, sizeof(PackageHeader));
	assert(status == File::Error::SUCCESS);

	// close file.
	status = File::Close(packFile);
	assert(status == File::Error::SUCCESS);

	// free up.
	delete[] buffer;
	i = 0;
	while (i < j)
	{
		delete[] nameArray[i++];
	}

	delete[] nameArray;
}

// End of file
