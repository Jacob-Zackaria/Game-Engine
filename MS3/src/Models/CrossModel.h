#ifndef CROSS_MODEL_H
#define CROSS_MODEL_H

#include "Model.h"

namespace Azul
{
	class CrossModel : public Model
	{
	public:
		CrossModel(const char* const pModelFileName);

		CrossModel() = delete;
		CrossModel(const CrossModel&) = delete;
		CrossModel& operator = (CrossModel&) = delete;
		virtual ~CrossModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		// Data

	};
}

#endif