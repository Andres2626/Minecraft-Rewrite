#include "Layers/Stack.h"

namespace MC 
{
	namespace Layers
	{
		Stack::Stack()
		{ 
		
		}

		Stack::~Stack()
		{

		}

		void Stack::PushLayer(Layer *layer)
		{
			m_Layers.push_back(layer);
		}

		bool Stack::Init()
		{
			for (u32t i = 0; i < m_Layers.size(); i++) {
				if(!m_Layers[i]->IsVisible())
					continue;

				if (!m_Layers[i]->Init())
					return false;
			}

			return true;
		}

		void Stack::Finish()
		{
			for (Layer *layer : m_Layers)
				delete layer;

			m_Layers.clear();
		}

		void Stack::OnUpdate(Utils::Timestep &ts)
		{
			for (u32t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnUpdate(ts);
			}
		}

		void Stack::OnRender(float alpha)
		{
			for (u32t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnRender(alpha);
			}
		}

		void Stack::OnTick()
		{
			for (u32t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnTick();
			}
		}

		void Stack::OnSuspended()
		{
			for (u32t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnSuspended();
			}
		}

	}
}