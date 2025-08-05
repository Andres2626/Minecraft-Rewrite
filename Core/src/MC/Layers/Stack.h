#pragma once
#include "common.h"

#include "Default.h"

namespace MC 
{
	namespace Layers 
	{

		class MC_API Stack : public Layer 
		{
		private:
			std::vector<Layer*> m_Layers;
		public:
			Stack();
			~Stack();
		public:
			void PushLayer(Layer* layer);
		public:
			void Init() override;
		public:
			void OnUpdate(Utils::Timestep& ts) override;
			void OnEvent(Events::Event& ev) override;
			void OnRender() override;
			void OnTick() override;
			void OnSuspended() override;
		};

	}
}