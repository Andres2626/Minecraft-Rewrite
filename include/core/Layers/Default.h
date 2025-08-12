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
			virtual void Init() override;
			virtual void OnUpdate(Utils::Timestep& ts) override;
			virtual void OnEvent(Events::Event& ev) override;
			virtual void OnRender() override;
			virtual void OnTick() override;
			virtual void OnSuspended() override;
		};
	}
}