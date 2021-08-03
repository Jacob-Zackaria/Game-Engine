#ifndef FBX_STL_H
#define FBX_STL_H

// Keenan(3.2.4) - STL

#include <assert.h>
#include <vector>
#include <algorithm>

struct FBX_Vertex
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
};


struct FBX_Vertex_vsn
{
	int index;
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;


	// Keenan(3.2.5) - VBO and Reduce
		bool operator == (const FBX_Vertex_vsn &b)
		{
			return (
				(this->x == b.x) &&
				(this->y == b.y) &&
				(this->z == b.z) &&
				(this->s == b.s) &&
				(this->t == b.t) &&
				(this->nx == b.nx) &&
				(this->ny == b.ny) &&
				(this->nz == b.nz)
				);
		}
		bool operator < (const FBX_Vertex_vsn &b)
		{
			if (this->x < b.x)
			{
				return true;
			}
			else if (this->x > b.x)
			{
				return false;
			}

			if (this->y < b.y)
			{
				return true;
			}
			else if (this->y > b.y)
			{
				return false;
			}

			if (this->z < b.z)
			{
				return true;
			}
			else if (this->z > b.z)
			{
				return false;
			}

			if (this->s < b.s)
			{
				return true;
			}
			else if (this->s > b.s)
			{
				return false;
			}

			if (this->t < b.t)
			{
				return true;
			}
			else if (this->t > b.t)
			{
				return false;
			}

			if (this->nx < b.nx)
			{
				return true;
			}
			else if (this->nx > b.nx)
			{
				return false;
			}

			if (this->ny < b.ny)
			{
				return true;
			}
			else if (this->ny > b.ny)
			{
				return false;
			}

			if (this->nz < b.nz)
			{
				return true;
			}
			else if (this->nz > b.nz)
			{
				return false;
			}
			else
			{
				return false;
			}
		}

};

struct FBX_Trilist
{
	int v0;
	int v1;
	int v2;
};


#endif