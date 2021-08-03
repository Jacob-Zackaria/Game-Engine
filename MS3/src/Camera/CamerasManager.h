#ifndef CAMERAS_MANAGER_H
#define CAMERAS_MANAGER_H

#include "Camera.h"
namespace Azul
{
	class CamerasManager
	{
	public:
		static void Add(Camera* pCamera, Camera::Name name);
		static Camera* Find(Camera::Name name);
		static void SetCurrent3D(Camera::Name name);
		static void SetCurrent2D(Camera::Name name);
		static void Create();
		static void Destroy();
		static void Update3D();
		static void Update2D();
		static Camera* GetActive3DCam();
		static void Next3DCam();
		static Camera* GetActive2DCam();
		static void Next2DCam();
		static void GetActiveHelper(Vect& up, Vect& tar, Vect& pos, Vect& upNorm, Vect& forwardNorm, Vect& rightNorm);
		static void SetActiveHelper(Vect& up, Vect& tar, Vect& pos);

	private:  // methods

		static CamerasManager* privGetInstance();
		CamerasManager();

		const bool Exist3DCam() const;
		const bool Exist2DCam() const;
		void privAddToFront(Camera* pNewData, Camera*& pHead);
		void privRemove(Camera* pNewData, Camera*& pHead);

	private:  // add
		Camera* active3D;
		Camera* active2D;
		Camera* camHead;
	};
}

#endif