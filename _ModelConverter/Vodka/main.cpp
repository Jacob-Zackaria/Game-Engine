//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "ChunkHeader.h"
#include "File.h"

//---------------------------------------------------------------------------
// Leave this signature - do your Vodka magic here
// Feel free to add files and methods to this project
//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	if (argc < 7)
	{
		printf("\nError : Fewer arguments.");
		return (-1);
	}
	else if (argc > 7)
	{
		printf("\nError : Extra arguments.");
		return(-1);
	}

	unsigned short int ctype = 0, ntype = 0;
	//check command.
	if (memcmp(argv[3], "-t", 2) == 0 || memcmp(argv[3], "-T", 2) == 0)
	{
		ctype = 4;
		if (memcmp(argv[5], "-n", 2) == 0 || memcmp(argv[5], "-N", 2) == 0)
		{
			ntype = 6;
		}
		else
		{
			printf("\nError: invalid command");
			return(-1);
		}
		
	}
	else if (memcmp(argv[3], "-n", 2) == 0 || memcmp(argv[3], "-N", 2) == 0)
	{
		ntype = 4;
		if (memcmp(argv[5], "-t", 2) == 0 || memcmp(argv[5], "-T", 2) == 0)
		{
			ctype = 6;
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

	// Create chunk header.
	ChunkHeader chunkHdr = {};
	
	// Set type.
	if (memcmp(argv[ctype], "UV_TYPE", 7) == 0)
	{
		chunkHdr.type = ChunkType::UV_TYPE;
	}
	else if (memcmp(argv[ctype], "VERTS_TYPE", 10) == 0)
	{
		chunkHdr.type = ChunkType::VERTS_TYPE;
	}
	else if (memcmp(argv[ctype], "ANIM_TYPE", 9) == 0)
	{
		chunkHdr.type = ChunkType::ANIM_TYPE;
	}
	else if (memcmp(argv[ctype], "NORMS_TYPE", 10) == 0)
	{
		chunkHdr.type = ChunkType::NORMS_TYPE;
	}
	else if (memcmp(argv[ctype], "TEXTURE_TYPE", 12) == 0)
	{
		chunkHdr.type = ChunkType::TEXTURE_TYPE;
	}
	else if (memcmp(argv[ctype], "POLY_TYPE", 9) == 0)
	{
		chunkHdr.type = ChunkType::POLY_TYPE;
	}
	else
	{
		printf("\n Type error");
		return(-1);
	}

	// Set name.
	//Find Length.
	unsigned int i = 0;
	char* traverse = argv[ntype];
	while (traverse[i] != '\0' && i < (ChunkHeader::CHUNK_NAME_SIZE - 1))
	{
		i++;
	}

	if (i >= (ChunkHeader::CHUNK_NAME_SIZE - 1))
	{
		printf("\nLong name not allowed.");
		return(-1);
	}
	else if (i == 0)
	{
		printf("\nError: Unknown name.");
		return(-1);
	}
	memcpy_s(chunkHdr.chunkName, ChunkHeader::CHUNK_NAME_SIZE, argv[ntype], i + 1);

	
	
	
	//Read from file.
	// File open.
	File::Error status;
	File::Handle fh = nullptr;
	status = File::Open(fh, argv[1], File::Mode::READ);
	assert(status == File::Error::SUCCESS);

	//create a buffer.
	char* buffer = new char[40 * 1024 * 1024];
	DWORD fileSize = 0;
	status = File::Seek(fh, File::Location::END, 0);
	assert(status == File::Error::SUCCESS);

	status = File::Tell(fh, fileSize);
	assert(status == File::Error::SUCCESS);

	status = File::Seek(fh, File::Location::BEGIN, 0);
	assert(status == File::Error::SUCCESS);

	status = File::Read(fh, buffer, fileSize);
	assert(status == File::Error::SUCCESS);

	// chunk size.
	chunkHdr.chunkSize = fileSize;

	//chunk hash.
	chunkHdr.hashNum = File::FindHash(buffer, fileSize);
	
	//close file.
	status = File::Close(fh);
	assert(status == File::Error::SUCCESS);

	//Write to .blo file
	File::Handle fh2 = nullptr;
	status = File::Open(fh2, argv[2], File::Mode::WRITE);
	assert(status == File::Error::SUCCESS);

	status = File::Seek(fh2, File::Location::BEGIN, 0);
	assert(status == File::Error::SUCCESS);

	// write header.
	status = File::Write(fh2, &chunkHdr, sizeof(ChunkHeader));
	assert(status == File::Error::SUCCESS);

	// write data.
	status = File::Write(fh2, buffer, fileSize);
	assert(status == File::Error::SUCCESS);

	//close file.
	status = File::Close(fh2);
	assert(status == File::Error::SUCCESS);

	// delete buffer.
	delete[] buffer;
}

// End of File
