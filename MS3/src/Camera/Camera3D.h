#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include "Camera.h"

namespace Azul
{
	class Camera3D : public Camera
	{
	public:
		Camera3D();
		Camera3D(const Camera3D&) = delete;
		Camera3D& operator = (const Camera3D&) = delete;
		virtual ~Camera3D() = default;

		virtual void privUpdateProjectionMatrix(void) override;
		void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
	};
}

#endif
