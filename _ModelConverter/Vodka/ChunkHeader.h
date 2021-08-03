//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

enum class ChunkType : uint32_t
{
	VERTS_TYPE = 0xA0000000u,
	POLY_TYPE,
	NORMS_TYPE,
	ANIM_TYPE,
	TEXTURE_TYPE,
	UV_TYPE
};

struct ChunkHeader
{
	static const unsigned int CHUNK_NAME_SIZE = 20;

	// data:
	ChunkType      type;
	char           chunkName[CHUNK_NAME_SIZE];
	unsigned int   chunkSize;
	unsigned int   hashNum;
};

#endif