#pragma once
#include "common.h"

#include "Layer.h"

namespace MC 
{
	namespace Layers 
	{

		class MC_API Default : public Layer 
		{
		public:
			Default();
			~Default();
		public:
			virtual bool Init() override;
			virtual void OnUpdate(Utils::Timestep &ts) override;
			virtual void OnRender(float alpha) override;
			virtual void OnTick() override;
			virtual void OnSuspended() override;
		};
	}
}