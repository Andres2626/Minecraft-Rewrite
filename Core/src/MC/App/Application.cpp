#include "App/Application.h"

#include "Input.h"

#include "Utils/Util.h"

#include <thread>

namespace MC 
{
	namespace App 
	{

		static Application* s_Instance = nullptr;

		Application::Application(const rd_str_t& name, const WindowProperties& pr)
			: m_FPS(0), m_UPS(0), m_FrameTime(0.0f), m_Name(name), m_Pr(pr), m_Running(false), m_Suspended(false)
		{
			s_Instance = this;
		} 

		Application::~Application()
		{
			if (m_Win)
				delete m_Win;
		}

		void Application::Init()
		{
			mc_info("MC Engine version: %s\n", MC_VERSION_STRING);
	
			m_Win = new Window(m_Name.c_str(), m_Pr);
			Input::Init();
		}

		void Application::PushLayer(Layers::Layer* layer)
		{
			this->m_LayerStack.PushLayer(layer);
		}

		void Application::Start()
		{
			m_Running = true;
			m_Suspended = false;

			Init();
			m_LayerStack.Init();
			Run();
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

		inline void Application::SetFPSGoal(int fps)
		{
			if (fps > 0) {
				m_FPSGoal = fps;
				m_Delay = 1000.0f / static_cast<float>(fps); // 1000ms / fps
			}
			else {
				m_FPSGoal = 0;
				m_Delay = 0.0f;
			}
		}

		void Application::Run()
		{
			Events::Event ev;
			this->m_Timer = new Utils::Timer();
			float ti = 0.0f;
			float update_timer = m_Timer->ElapsedMillis();
			float update_tick = 1000.0f / 60.0f;
			Utils::Timestep step(m_Timer->ElapsedMillis());
			while (m_Running) {
				Utils::Timer frametime;

				m_Win->Clear();
				float now = m_Timer->ElapsedMillis();
				while (now - update_timer >= update_tick) {
					step.Update(now);

					OnUpdate(step);

					m_UPS++;
					update_timer += update_tick;
				}

				OnRender();
				m_Win->Update();

				m_FPS++;
				m_FrameTime = frametime.ElapsedMillis();

				if (now - ti > 1000.0f) {
					ti += 1000.0f;

					OnTick();

					m_FPS = 0;
					m_UPS = 0;
				}

				while (m_Suspended)
					this->OnSuspended();

				if (m_Win->Close())
					m_Running = false;

				this->OnEvent(ev);

				/* TODO: Improve this! */
				if (m_FPSGoal > 0) {
					float remaining = m_Delay - m_FrameTime;
					if (remaining > 0.0f)
						std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(remaining));
				}
			}
		}

		void Application::OnUpdate(Utils::Timestep& ts)
		{
			m_LayerStack.OnUpdate(ts);
		}

		void Application::OnEvent(Events::Event& ev)
		{
			m_LayerStack.OnEvent(ev);
		}

		void Application::OnRender()
		{

			m_LayerStack.OnRender();
		}

		void Application::OnTick()
		{
			m_LayerStack.OnTick();
		}

		void Application::OnSuspended()
		{
			m_LayerStack.OnSuspended();
		}

		Application& Application::Get() 
		{
			mc_assert(s_Instance, "application instance is null pointer\n");
			return *s_Instance;
		}
	}
}