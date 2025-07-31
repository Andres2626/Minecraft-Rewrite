#include "App/Application.h"
#include "Input.h"

namespace MC {
	namespace App {

		Application *Application::m_Instance = nullptr;

		Application::Application(const std::string& name, const WindowProperties& pr)
			: m_FPS(0), m_UPS(0), m_FrameTime(0.0f), m_Name(name), m_Pr(pr), m_Running(false), m_Suspended(false)
		{
			this->m_Instance = this;
		} 

		Application::~Application()
		{
			delete this->m_Win;
		}

		void Application::Init()
		{
			MC_INFO("Engine version: %s, code %i", RD_VERSION_STRING, RD_VERSION_NUMBER);
			this->m_Win = new Window(this->m_Name.c_str(), this->m_Pr);
			Input::Init();
		}

		void Application::PushLayer(Layer::DefaultLayer* layer)
		{
			this->m_LayerStack.push_back(layer);
		}

		Layer::DefaultLayer* Application::PopLayer()
		{
			Layer::DefaultLayer* layer = this->m_LayerStack.back();
			m_LayerStack.pop_back();
			return layer;
		}

		Layer::DefaultLayer* Application::PopLayer(Layer::DefaultLayer* layer)
		{
			for (int i = 0; i < this->m_LayerStack.size(); i++) {
				if (this->m_LayerStack[i] == layer) {
					this->m_LayerStack.erase(this->m_LayerStack.begin() + i);
					break;
				}
			}
			return layer;
		}

		void Application::Start()
		{
			this->m_Running = true;
			this->m_Suspended = false;

			this->Init();

			/* Init layers */
			for (int i = 0; i < this->m_LayerStack.size(); i++) {
				if (m_LayerStack[i]->IsVisible())
					m_LayerStack[i]->Init();
			}

			this->Run();
		}

		void Application::Suspend()
		{
			this->m_Suspended = true;
		}

		void Application::Resume()
		{
			this->m_Suspended = false;
		}

		void Application::Stop()
		{
			this->m_Running = false;
		}

		void Application::Run()
		{
			Events::Event ev;
			this->m_Timer = new Utils::Timer();
			float ti = 0.0f;
			float update_timer = this->m_Timer->ElapsedMillis();
			float update_tick = 1000.0f / 60.0f;
			Utils::Timestep step(this->m_Timer->ElapsedMillis());
			while (this->m_Running) {
				this->m_Win->Clear();
				float now = this->m_Timer->ElapsedMillis();
				while (now - update_timer >= update_tick) {
					step.Update(now);

					/* Update game*/
					this->OnUpdate(step);

					this->m_UPS++;
					update_timer += update_tick;
				}

				this->m_FPS++;

				Utils::Timer frametime;
				this->m_FrameTime = frametime.ElapsedMillis();

				this->OnRender();
				this->m_Win->Update();

				if (now - ti > 1000.0f) {
					ti += 1000.0f;

					/* OnTick process */
					this->OnTick();

					/* reset FPS and UPS */
					this->m_FPS = 0;
					this->m_UPS = 0;
				}

				while (this->m_Suspended) {
					this->OnSuspended();
				}

				if (this->m_Win->Close())
					this->m_Running = false;

				this->OnEvent(ev);
			}
		}

		void Application::OnUpdate(Utils::Timestep& ts)
		{
			for (int i = 0; i < this->m_LayerStack.size(); i++) {
				if (m_LayerStack[i]->IsVisible())
					m_LayerStack[i]->OnUpdate(ts);
			}
		}

		void Application::OnEvent(Events::Event& ev)
		{
			for (int i = 0; i < this->m_LayerStack.size(); i++) {
				if (m_LayerStack[i]->IsVisible())
					m_LayerStack[i]->OnEvent(ev);
			}
		}

		void Application::OnRender()
		{
			for (int i = 0; i < this->m_LayerStack.size(); i++) {
				if (m_LayerStack[i]->IsVisible())
					m_LayerStack[i]->OnRender();
			}
		}

		void Application::OnTick()
		{
			for (int i = 0; i < this->m_LayerStack.size(); i++) {
				if (m_LayerStack[i]->IsVisible())
					m_LayerStack[i]->OnTick();
			}
		}
		void Application::OnSuspended()
		{
			for (int i = 0; i < this->m_LayerStack.size(); i++) {
				if (m_LayerStack[i]->IsVisible())
					m_LayerStack[i]->OnSuspended();
			}
		}
	}
}