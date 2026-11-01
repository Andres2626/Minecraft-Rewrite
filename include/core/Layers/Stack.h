#pragma once
#include "common.h"

#include "Default.h"

#include "Utils/Util.h"

namespace MC 
{
	namespace Layers 
	{

		class MC_API Stack : public Layer 
		{
		private:
			struct PImpl;
			PImpl *m_Impl;
		public:
			Stack();
			~Stack();
		public:
			void PushLayer(Layer *layer);
		public:
			bool Init() override;
			void Finish();
		public:
			void OnUpdate(Utils::Timestep &ts) override;
			void OnRender(float alpha) override;
			void OnTick() override;
			void OnSuspended() override;
		};

	}
}