#pragma once
#include "common.h"

#include "Events/Event.h"
#include "Utils/Timestep.h"

namespace MC {
	namespace Layer {

		class DefaultLayer {
		private:
			bool m_Visible;
		public:
			DefaultLayer();
			~DefaultLayer();
		public:
			virtual void Init();
			virtual void OnUpdate(Utils::Timestep& ts);
			virtual void OnEvent(Events::Event& ev);
			virtual void OnRender();
			virtual void OnTick();
			virtual void OnSuspended();
		public:
			inline bool IsVisible() { return this->m_Visible; }
			inline void SetVisible(bool visible) { this->m_Visible = visible; }
		};
	}
}