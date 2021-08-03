#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include "Camera.h"

namespace Azul
{
	class Camera2D : public Camera
	{
	public:
		Camera2D();
		Camera2D(const Camera2D&) = delete;
		Camera2D& operator = (const Camera2D&) = delete;
		virtual ~Camera2D() = default;

		virtual void privUpdateProjectionMatrix(void) override;
		void setOrthographic(const float xMin, const float xMax, const float yMin, const float yMax, const float zMin, const float zMax);

	private:

		// ortho
		float	xMin;
		float	yMin;
		float	zMin;
		float	xMax;
		float	yMax;
		float	zMax;
	};
}

#endif
