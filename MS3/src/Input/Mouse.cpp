//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GameManager.h"
#include "Mouse.h"

namespace Azul
{

	Mouse::Mouse()
	{

	}

	// Use this to read mouse buttons
	bool Mouse::GetKeyState(Button button)
	{
		bool value;

		if (GameManager::GetGame()->GetMouseButton(button) == GLFW_PRESS)
		{
			value = true;
		}
		else
		{
			value = false;
		}
		return value;
	}

	void Mouse::GetCursor(float& xPos, float& yPos)
	{
		// get mouse position
		GameManager::GetGame()->GetCursorPos(xPos, yPos);

	}

}
// --- End of File ---