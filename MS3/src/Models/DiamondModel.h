#ifndef DIAMOND_MODEL_H
#define DIAMOND_MODEL_H

#include "Model.h"

namespace Azul
{
	class DiamondModel : public Model
	{
	public:
		DiamondModel(const char* const pModelFileName);

		DiamondModel() = delete;
		DiamondModel(const DiamondModel&) = delete;
		DiamondModel& operator = (DiamondModel&) = delete;
		virtual ~DiamondModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif