#ifndef PYRAMID_MODEL_H
#define PYRAMID_MODEL_H

#include "Model.h"

namespace Azul
{
	class PyramidModel : public Model
	{
	public:
		PyramidModel(const char* const pModelFileName);

		PyramidModel() = delete;
		PyramidModel(const PyramidModel&) = delete;
		PyramidModel& operator = (PyramidModel&) = delete;
		virtual ~PyramidModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif