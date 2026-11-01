#include "mc.h"

#include "Layers/Stack.h"

namespace MC 
{
	namespace Layers
	{
		struct Stack::PImpl
		{
			std::vector<Layer*> Layers;
		};

		Stack::Stack()
		{ 
			m_Impl = new PImpl();
		}

		Stack::~Stack()
		{
			if (m_Impl)
				delete m_Impl;
		}

		void Stack::PushLayer(Layer *layer)
		{
			m_Impl->Layers.push_back(layer);
		}

		bool Stack::Init()
		{
			for (u32t i = 0; i < m_Impl->Layers.size(); i++) {
				if(!m_Impl->Layers[i]->IsVisible())
					continue;

				if (!m_Impl->Layers[i]->Init())
					return false;
			}

			return true;
		}

		void Stack::Finish()
		{
			for (Layer *layer : m_Impl->Layers)
				delete layer;

			m_Impl->Layers.clear();
		}

		void Stack::OnUpdate(Utils::Timestep &ts)
		{
			for (u32t i = 0; i < m_Impl->Layers.size(); i++) {
				if (m_Impl->Layers[i]->IsVisible())
					m_Impl->Layers[i]->OnUpdate(ts);
			}
		}

		void Stack::OnRender(float alpha)
		{
			for (u32t i = 0; i < m_Impl->Layers.size(); i++) {
				if (m_Impl->Layers[i]->IsVisible())
					m_Impl->Layers[i]->OnRender(alpha);
			}
		}

		void Stack::OnTick()
		{
			for (u32t i = 0; i < m_Impl->Layers.size(); i++) {
				if (m_Impl->Layers[i]->IsVisible())
					m_Impl->Layers[i]->OnTick();
			}
		}

		void Stack::OnSuspended()
		{
			for (u32t i = 0; i < m_Impl->Layers.size(); i++) {
				if (m_Impl->Layers[i]->IsVisible())
					m_Impl->Layers[i]->OnSuspended();
			}
		}

	}
}