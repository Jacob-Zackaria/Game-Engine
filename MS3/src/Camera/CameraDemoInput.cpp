//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CameraDemoInput.h"
#include "InputManager.h"
#include "Keyboard.h"
#include "CamerasManager.h"

namespace Azul
{
	void CameraDemoInput()
	{
		Keyboard* key = InputManager::GetKeyboard();

		if (key->GetKeyState(Keyboard::KEY_SPACE))
		{
			Camera* pCamera = CamerasManager::GetActive3DCam();

			if (pCamera->getName() == Camera::Name::CAMERA_0)
			{
				CamerasManager::SetCurrent3D(Camera::Name::CAMERA_1);
			}
			else
			{
				CamerasManager::SetCurrent3D(Camera::Name::CAMERA_0);
			}
		}

		

		if (key->GetKeyState(Keyboard::KEY_0))
		{
			CamerasManager::SetCurrent3D(Camera::Name::CAMERA_0);
		}

		if (key->GetKeyState(Keyboard::KEY_1))
		{
			CamerasManager::SetCurrent3D(Camera::Name::CAMERA_1);
		}

		if (key->GetKeyState(Keyboard::KEY_2))
		{
			CamerasManager::SetCurrent3D(Camera::Name::CAMERA_2);
		}

		if (key->GetKeyState(Keyboard::KEY_3))
		{
			CamerasManager::SetCurrent3D(Camera::Name::CAMERA_3);
		}
	}

}

// --- End of File ---