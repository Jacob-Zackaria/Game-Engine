//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SSBO.h"

namespace Azul
{
	SSBO::SSBO()
		:
		ssbo(),
		elementCount(0),
		elementSize(0)
	{
		glGenBuffers(1, &ssbo);
	}


	SSBO::~SSBO()
	{
		// Tell GPU it can be released
		glDeleteBuffers(1, &ssbo);
	}

	void SSBO::Set(unsigned _elementCount, unsigned _size, void* pData)
	{
		this->elementCount = _elementCount;
		this->elementSize = _size;

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(elementCount * this->elementSize), pData, GL_STATIC_DRAW);
	}

	unsigned int SSBO::GetLocation() const
	{
		return this->ssbo;
	}

	void SSBO::Bind(unsigned int bindLocation)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindLocation, this->ssbo);
	}

	void* SSBO::Map(SSBO::Access accessType)
	{
		void* result = glMapNamedBuffer(this->ssbo, GLenum(accessType));
		assert(result);

		return result;
	}

	void SSBO::Unmap()
	{
		glUnmapNamedBuffer(this->ssbo);
	}
}

//--- End of File ----