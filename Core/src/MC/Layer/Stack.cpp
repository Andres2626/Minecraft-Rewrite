#include "Stack.h"

namespace MC {
	namespace Layer {

		Stack::Stack()
		{ 
		
		}

		Stack::~Stack()
		{

		}

		void Stack::PushLayer(Layer::ILayer* layer)
		{
			this->m_Layers.push_back(layer);
		}

		ILayer* Stack::PopLayer()
		{
			Layer::ILayer* layer = this->m_Layers.back();
			this->m_Layers.pop_back();
			return layer;
		}

		ILayer* Stack::PopLayer(Layer::ILayer* layer)
		{
			for (rd_size_t i = 0; i < this->m_Layers.size(); i++) {
				if (this->m_Layers[i] == layer) {
					this->m_Layers.erase(this->m_Layers.begin() + i);
					break;
				}
			}
			return layer;
		}

		void Stack::Init()
		{
			for (rd_size_t i = 0; i < this->m_Layers.size(); i++) {
				if (this->m_Layers[i]->IsVisible())
					this->m_Layers[i]->Init();
			}
		}

		void Stack::OnUpdate(Utils::Timestep& ts)
		{
			for (rd_size_t i = 0; i < this->m_Layers.size(); i++) {
				if (this->m_Layers[i]->IsVisible())
					this->m_Layers[i]->OnUpdate(ts);
			}
		}

		void Stack::OnEvent(Events::Event& ev)
		{
			for (rd_size_t i = 0; i < this->m_Layers.size(); i++) {
				if (this->m_Layers[i]->IsVisible())
					this->m_Layers[i]->OnEvent(ev);
			}
		}

		void Stack::OnRender()
		{
			for (rd_size_t i = 0; i < this->m_Layers.size(); i++) {
				if (this->m_Layers[i]->IsVisible())
					this->m_Layers[i]->OnRender();
			}
		}

		void Stack::OnTick()
		{
			for (rd_size_t i = 0; i < this->m_Layers.size(); i++) {
				if (this->m_Layers[i]->IsVisible())
					this->m_Layers[i]->OnTick();
			}
		}

		void Stack::OnSuspended()
		{
			for (rd_size_t i = 0; i < this->m_Layers.size(); i++) {
				if (this->m_Layers[i]->IsVisible())
					this->m_Layers[i]->OnSuspended();
			}
		}

	}
}