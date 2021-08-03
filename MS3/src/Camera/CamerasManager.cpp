#include "CamerasManager.h"
namespace Azul
{
	CamerasManager::CamerasManager()
		:
		active3D(nullptr),
		active2D(nullptr),
		camHead(nullptr)
	{
	}

	CamerasManager* CamerasManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static CamerasManager cameraMan;
		return &cameraMan;
	}


	void CamerasManager::Create()
	{
		CamerasManager* pCameraMan;
		pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);
	}

	void CamerasManager::Destroy()
	{
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		Camera* pLink = pCameraMan->camHead;

		while (pLink != nullptr)
		{
			Camera* pTmp = pLink;
			pLink = (Camera*)pLink->getNext();
			pCameraMan->privRemove(pTmp, pCameraMan->camHead);
			delete pTmp;
		}
	}

	void CamerasManager::Add(Camera* pCamera, Camera::Name name)
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		
		assert(pCamera);
		pCamera->setName(name);

		// Now add it to the manager
		pCameraMan->privAddToFront(pCamera, pCameraMan->camHead);
	}

	Camera* CamerasManager::Find(Camera::Name name)
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		Camera* pLink = pCameraMan->camHead;

		while (pLink != nullptr)
		{
			if (pLink->getName() == name)
			{
				return pLink;
			}
			pLink = (Camera*)pLink->getNext();
		}

		return nullptr;
	}

	void CamerasManager::SetCurrent3D(const Camera::Name name)
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		Camera* pCam = CamerasManager::Find(name);
		assert(pCam->getType() == Camera::Type::PERSPECTIVE_3D);


		pCameraMan->active3D = pCam;
	}

	void CamerasManager::SetCurrent2D(const Camera::Name name)
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		Camera* pCam = CamerasManager::Find(name);
		assert(pCam->getType() == Camera::Type::ORTHOGRAPHIC_2D);

		pCameraMan->active2D = pCam;
	}

	void CamerasManager::Update3D()
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		assert(pCameraMan->active3D != nullptr);
		pCameraMan->active3D->updateCamera();
	}

	void CamerasManager::Update2D()
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		assert(pCameraMan->active2D != nullptr);
		pCameraMan->active2D->updateCamera();
	}

	Camera* CamerasManager::GetActive3DCam()
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		return (pCameraMan->active3D);
	}

	Camera* CamerasManager::GetActive2DCam()
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		return (pCameraMan->active2D);
	}

	void CamerasManager::Next3DCam()
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		if (pCameraMan->Exist3DCam())
		{
			Camera* currCam = pCameraMan->active3D;
			Camera* pNextCam = currCam->getNext();

			while (pNextCam != currCam)
			{
				if (pNextCam == nullptr)
				{
					pNextCam = pCameraMan->camHead;
				}

				if (pNextCam->getType() == Camera::Type::PERSPECTIVE_3D)
				{
					pCameraMan->active3D = pNextCam;
					return;
				}

				pNextCam = pNextCam->getNext();
			}
		}
	}

	void CamerasManager::Next2DCam()
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		if (pCameraMan->Exist2DCam())
		{
			Camera* currCam = pCameraMan->active2D;
			Camera* pNextCam = currCam->getNext();

			while (pNextCam != currCam)
			{
				if (pNextCam == nullptr)
				{
					pNextCam = pCameraMan->camHead;
				}

				if (pNextCam->getType() == Camera::Type::ORTHOGRAPHIC_2D)
				{
					pCameraMan->active2D = pNextCam;
					return;
				}

				pNextCam = pNextCam->getNext();
			}
		}
	}

	const bool CamerasManager::Exist3DCam() const
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		assert(pCameraMan->camHead != nullptr);

		Camera* pCam = pCameraMan->camHead;
		while (pCam != nullptr)
		{
			if (pCam->getType() == Camera::Type::PERSPECTIVE_3D)
			{
				return true;
			}
			pCam = pCam->getNext();
		}
		return false;
	}


	const bool CamerasManager::Exist2DCam() const
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		assert(pCameraMan->camHead != nullptr);

		Camera* pCam = pCameraMan->camHead;
		while (pCam != nullptr)
		{
			if (pCam->getType() == Camera::Type::ORTHOGRAPHIC_2D)
			{
				return true;
			}
			pCam = pCam->getNext();
		}
		return false;
	}

	void CamerasManager::GetActiveHelper(Vect& up, Vect& tar, Vect& pos, Vect& upNorm, Vect& forwardNorm, Vect& rightNorm)
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		pCameraMan->active3D->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);
	}
	void CamerasManager::SetActiveHelper(Vect& up, Vect& tar, Vect& pos)
	{
		// Get the instance to the manager
		CamerasManager* pCameraMan = CamerasManager::privGetInstance();
		assert(pCameraMan);

		pCameraMan->active3D->SetHelper(up, tar, pos);
	}

	void CamerasManager::privAddToFront(Camera* pNewData, Camera*& pHead)
	{
		//Check if data to add is not null.
		assert(pNewData != nullptr);

		//If head is null.
		if (pHead == nullptr)
		{
			pNewData->setNext(nullptr);
			pNewData->setPrev(nullptr);
		}
		else        //If head is not null.
		{
			pNewData->setNext(pHead);
			pNewData->setPrev(nullptr);
			pHead->setPrev(pNewData);
		}

		//Assign head as the added data.
		pHead = pNewData;
	}

	void CamerasManager::privRemove(Camera* pNewData, Camera*& pHead)
	{
		//Check if node to delete has a next or prev links.
		if (pNewData->getNext() == nullptr && pNewData->getPrev() == nullptr)
		{
			pHead = nullptr;
		}
		else if (pNewData->getPrev() == nullptr)    //Check if there is a prev link. 
		{
			pHead = pHead->getNext();
			pHead->setPrev(nullptr);
		}
		else if (pNewData->getNext() == nullptr)    //Check if there is a next link.
		{
			pNewData->getPrev()->setNext(nullptr);
		}
		else                    //If no next or prev links are present.
		{
			pNewData->getPrev()->setNext(pNewData->getNext());
			pNewData->getNext()->setPrev(pNewData->getPrev());
		}

		//Clear links of deleted node.
		pNewData->setNext(nullptr);
		pNewData->setPrev(nullptr);

	}
}