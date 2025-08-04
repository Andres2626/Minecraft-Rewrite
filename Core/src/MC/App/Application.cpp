#include "App/Application.h"
#include "Input.h"

namespace MC {
	namespace App {

		static Application* m_Instance = nullptr;

		Application::Application(const rd_str_t& name, const WindowProperties& pr)
			: m_FPS(0), m_UPS(0), m_FrameTime(0.0f), m_Name(name), m_Pr(pr), m_Running(false), m_Suspended(false)
		{
			m_Instance = this;
		} 

		Application::~Application()
		{
			delete this->m_Win;
		}

		void Application::Init()
		{
			RD_INFO << "Engine version: " << RD_VERSION_STRING << ", code", RD_VERSION_NUMBER;

			this->m_Win = new Window(this->m_Name.c_str(), this->m_Pr);
			Input::Init();
		}

		void Application::PushLayer(Layer::ILayer* layer)
		{
			this->m_LayerStack.PushLayer(layer);
		}

		Layer::ILayer* Application::PopLayer()
		{
			return this->m_LayerStack.PopLayer();
		}

		Layer::ILayer* Application::PopLayer(Layer::ILayer* layer)
		{
			return this->m_LayerStack.PopLayer(layer);
		}

		void Application::Start()
		{
			this->m_Running = true;
			this->m_Suspended = false;

			this->Init();
			this->m_LayerStack.Init();
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
			this->m_LayerStack.OnUpdate(ts);
		}

		void Application::OnEvent(Events::Event& ev)
		{
			this->m_LayerStack.OnEvent(ev);
		}

		void Application::OnRender()
		{
			this->m_LayerStack.OnRender();
		}

		void Application::OnTick()
		{
			this->m_LayerStack.OnTick();
		}

		void Application::OnSuspended()
		{
			this->m_LayerStack.OnSuspended();
		}

		Application& Application::GetInstance() 
		{
			return *m_Instance;
		}
	}
}