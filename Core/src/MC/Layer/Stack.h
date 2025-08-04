#pragma once
#include "common.h"

#include "Default.h"

namespace MC {
	namespace Layer {

		class RD_API Stack : public ILayer {
		private:
			std::vector<ILayer*> m_Layers;
		public:
			Stack();
			~Stack();
		public:
			void PushLayer(ILayer* layer);
			ILayer* PopLayer();
			ILayer* PopLayer(ILayer* layer);
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