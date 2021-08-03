#include "Model.h"

namespace Azul
{
	Model::Model()
		: 
		numVerts(0),
		numTris(0),   
		vao(0),
		vbo_verts(0),
		vbo_trilist(0),
		poRefSphere(new Sphere())
	{
	}

	Model::~Model()
	{
		delete this->poRefSphere;
	}
}