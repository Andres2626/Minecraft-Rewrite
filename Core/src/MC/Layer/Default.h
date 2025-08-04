#pragma once
#include "common.h"

#include "ILayer.h"

namespace MC 
{
	namespace Layer 
	{

		class MC_API Default : public ILayer 
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