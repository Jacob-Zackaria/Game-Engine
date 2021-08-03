//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ShaderObject.h"

namespace Azul
{
	class ShaderManager
	{
	public:
		static void Add(ShaderObject* pShader);
		static void Create();
		static void Destroy();
		static ShaderObject* Find(ShaderObject::Name name);
	private:  // methods

		static ShaderManager* privGetInstance();
		ShaderManager();

		void privAddToFront(ShaderObject* node, ShaderObject*& head);
		void privRemove(ShaderObject* node, ShaderObject*& head);

	private:  // add

		ShaderObject* active;
	};

}

#endif

// ---  End of File ---
