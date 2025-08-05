#include "Stack.h"

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
			for (rd_size_t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->Init();
			}
		}

		void Stack::OnUpdate(Utils::Timestep& ts)
		{
			for (rd_size_t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnUpdate(ts);
			}
		}

		void Stack::OnEvent(Events::Event& ev)
		{
			for (rd_size_t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnEvent(ev);
			}
		}

		void Stack::OnRender()
		{
			for (rd_size_t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnRender();
			}
		}

		void Stack::OnTick()
		{
			for (rd_size_t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnTick();
			}
		}

		void Stack::OnSuspended()
		{
			for (rd_size_t i = 0; i < m_Layers.size(); i++) {
				if (m_Layers[i]->IsVisible())
					m_Layers[i]->OnSuspended();
			}
		}

	}
}