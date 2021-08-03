//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PACKAGE_HEADER_H
#define PACKAGE_HEADER_H

struct PackageHeader
{
	static const unsigned int PACKAGE_VERSION_SIZE = 20;
	static const unsigned int PACKAGE_NAME_SIZE = 20;


	char			packageName[PACKAGE_NAME_SIZE];
	char			versionString[PACKAGE_VERSION_SIZE];
	unsigned int	numChunks;
	unsigned int	totalSize;   // size of file (without package header)
	unsigned int	numVerts;
	unsigned int	numTri;
	bool			texAvailable;
};

#endif

// End of File
