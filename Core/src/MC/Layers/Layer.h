#pragma once
#include "common.h"

#include "Events/Event.h"
#include "Utils/Timestep.h"

namespace MC 
{
	namespace Layers 
	{

		class MC_API Layer 
		{
		private:
			bool m_Visible;
		public:
			virtual void Init() = 0;
			virtual void OnUpdate(Utils::Timestep& ts) = 0;
			virtual void OnEvent(Events::Event& ev) = 0;
			virtual void OnRender() = 0;
			virtual void OnTick() = 0;
			virtual void OnSuspended() = 0;
		public:
			inline bool IsVisible() { return m_Visible; }
			inline void SetVisible(bool visible) { m_Visible = visible; }
		};

	}
}