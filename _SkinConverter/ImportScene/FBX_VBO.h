#ifndef FBX_VBO_H
#define FBX_VBO_H

// Keenan(3.2.5) - VBO and Reduce

struct VBO_Trilist
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
};


struct VBO_Vertex_vsn
{
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
};

#endif