#ifndef MILITARY_MODEL_H
#define MILITARY_MODEL_H

#include "Model.h"

namespace Azul
{
	class MilitaryModel : public Model
	{
	public:
		MilitaryModel(const char* const pModelFileName);

		MilitaryModel() = delete;
		MilitaryModel(const MilitaryModel&) = delete;
		MilitaryModel& operator = (MilitaryModel&) = delete;
		virtual ~MilitaryModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif