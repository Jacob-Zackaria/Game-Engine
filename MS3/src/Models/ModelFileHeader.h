//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_FILE_HDR_H
#define MODEL_FILE_HDR_H


namespace Azul
{
	struct ModelFileHeader
	{
	public:
		static const unsigned int OBJECT_NAME_SIZE = 32;
		static const unsigned int TEXTURE_NAME_SIZE = 32;

	public:
		ModelFileHeader();
		ModelFileHeader(const ModelFileHeader&) = delete;
		ModelFileHeader& operator = (const ModelFileHeader&) = delete;
		~ModelFileHeader() = default;

	public:
		// Data -----------------

		// object name
		char	objName[OBJECT_NAME_SIZE];

		// vertex buffer
		int		numVerts;
		DWORD	vertBufferOffset;

		// trilist index 
		int		numTriList;
		DWORD	triListBufferOffset;

		// texture info
		// char textName[TEXTURE_NAME_SIZE];
		// unsigned int minFilter;
		// unsigned int magFilter;
		// unsigned int horizWrapMode;
		// unsigned int vertWrapMode;

	};


}

#endif

// --- End of File ---
