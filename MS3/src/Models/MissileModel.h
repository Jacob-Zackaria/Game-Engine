#ifndef MISSILE_MODEL_H
#define MISSILE_MODEL_H

#include "Model.h"

namespace Azul
{
	class MissileModel : public Model
	{
	public:
		MissileModel(const char* const pModelFileName);

		MissileModel() = delete;
		MissileModel(const MissileModel&) = delete;
		MissileModel& operator = (MissileModel&) = delete;
		virtual ~MissileModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif