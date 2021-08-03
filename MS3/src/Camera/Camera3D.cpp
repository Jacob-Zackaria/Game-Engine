#include "Camera3D.h"

namespace Azul
{
	Camera3D::Camera3D()
		: 
		Camera(Camera::Type::PERSPECTIVE_3D)
	{
	}

	// critical informational knobs for the perspective matrix
	// Field of View Y is in degrees (copying lame OpenGL)
	void Camera3D::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
	{
		this->aspectRatio = Aspect;
		this->fovy = Fovy;
		this->nearDist = NearDist;
		this->farDist = FarDist;
	}

	// The projection matrix (note it's invertable)
	void Camera3D::privUpdateProjectionMatrix(void)
	{
		this->projMatrix[m0] = 2.0f * this->nearDist / this->near_width;
		this->projMatrix[m1] = 0.0f;
		this->projMatrix[m2] = 0.0f;
		this->projMatrix[m3] = 0.0f;

		this->projMatrix[m4] = 0.0f;
		this->projMatrix[m5] = 2.0f * this->nearDist / this->near_height;
		this->projMatrix[m6] = 0.0f;
		this->projMatrix[m7] = 0.0f;

		this->projMatrix[m8] = 0.0f;
		this->projMatrix[m9] = 0.0f;
		this->projMatrix[m10] = (this->farDist + this->nearDist) / (this->nearDist - this->farDist);
		this->projMatrix[m11] = -1.0f;

		this->projMatrix[m12] = 0.0f;
		this->projMatrix[m13] = 0.0f;
		this->projMatrix[m14] = (2.0f * this->farDist * this->nearDist) / (this->nearDist - this->farDist);
		this->projMatrix[m15] = 0.0f;
	}
}