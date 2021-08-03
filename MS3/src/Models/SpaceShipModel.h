#ifndef SPACE_SHIP_MODEL_H
#define SPACE_SHIP_MODEL_H

#include "Model.h"

namespace Azul
{
	class SpaceShipModel : public Model
	{
	public:
		SpaceShipModel(const char* const pModelFileName);

		SpaceShipModel() = delete;
		SpaceShipModel(const SpaceShipModel&) = delete;
		SpaceShipModel& operator = (SpaceShipModel&) = delete;
		virtual ~SpaceShipModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif