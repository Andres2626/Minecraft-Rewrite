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

		void Stack::PushLayer(Layer* layer)
		{
			m_Layers.push_back(layer);
		}

		void Stack::Init()
		{
			for (mc_size i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->Init();
			}
		}

		void Stack::OnUpdate(Utils::Timestep& ts)
		{
			for (mc_size i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnUpdate(ts);
			}
		}

		void Stack::OnEvent(Events::Event& ev)
		{
			for (mc_size i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnEvent(ev);
			}
		}

		void Stack::OnRender()
		{
			for (mc_size i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnRender();
			}
		}

		void Stack::OnTick()
		{
			for (mc_size i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnTick();
			}
		}

		void Stack::OnSuspended()
		{
			for (mc_size i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnSuspended();
			}
		}

	}
}