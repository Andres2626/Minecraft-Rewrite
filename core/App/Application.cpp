#include "App/Application.h"

#include "App/Input.h"

#include "Events/EventManager.h"
#include "Utils/Util.h"
#include "Log/Log.h"

#ifdef MC_PLATFORM_WEB
#include <emscripten.h>

static void WebLoop()
{
    auto &app = MC::App::Application::Get();
    if (!app.IsRunning())
        return;
    app.Frame();
}

#endif /* MC_PLATFORM_WEB */

namespace MC 
{
	namespace App 
	{
		static Application* s_Instance = nullptr;

		Application::Application(const char *name, const WindowProperties &pr)
			: m_FPS(0), m_UPS(0), m_FrameTime(0.0f), m_Name(name), m_Pr(pr), m_Running(false), m_Suspended(false)
		{
			s_Instance = this;
		} 

		Application::~Application()
		{

		}

		bool Application::Init()
		{
			Log::Init(MC_LOG_STDOUT | MC_LOG_FILE, level_enum::info);
			mc_info("Minecraft rewrite engine version {}", MC_VERSION_STRING);

			m_Win = std::make_unique<Window>(m_Name, m_Pr);
			const Error &err = m_Win->GetError();
			if (err.num != ErrorType::NoError) {
				mc_error("Application error {}: {}", (u32t)err.num, err.str);
				return false;
			}

			Input::Init();

			return true;
		}

		void Application::PushLayer(Layers::Layer *layer)
		{
			this->m_LayerStack.PushLayer(layer);
		}

		void Application::Start()
		{
			if (!Init())
				return;

			if (!m_LayerStack.Init())
				return;

			m_Running = true;
			m_Suspended = false;
			
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
#ifdef MC_PLATFORM_WEB
            emscripten_cancel_main_loop();
#endif /* MC_PLATFORM_WEB */
		}

		void Application::Run()
		{
			m_TimeAcumulator = 0.0f;
			m_UpdateTimer = m_Timer.ElapsedMillis();
			m_UpdateTick = 1000.0f / 20.0f;
			m_Step = Utils::Timestep(m_UpdateTimer);
            
#ifndef MC_PLATFORM_WEB
			while (m_Running)
				Frame();
#else
            emscripten_set_main_loop(WebLoop, 0, true);
#endif /* !MC_PLATFORM_WEB */

            Stop();
		}

		void Application::Shutdown()
		{
			m_LayerStack.Finish();
			Input::Finish();
			Log::Finish();
		}

		void Application::Frame()
		{
			Utils::Timer frametime;
            
            if (m_Suspended) {
                OnSuspended();
                return;
            }
            
			DISPATCH_EVENTS();

			m_Win->Clear();
			float now = m_Timer.ElapsedMillis();
			while (now - m_UpdateTimer >= m_UpdateTick) {
				m_Step.Update(now);
				OnUpdate(m_Step);
				m_UPS++;
				m_UpdateTimer += m_UpdateTick;
			}

			float alpha = (now - m_UpdateTimer) / m_UpdateTick;

			OnRender(alpha);
			m_Win->Update();
			m_FPS++;
			m_FrameTime = frametime.ElapsedMillis();

			if (now - m_TimeAcumulator > 1000.0f) {
				m_TimeAcumulator += 1000.0f;
				OnTick();
				m_FPS = 0;
				m_UPS = 0;
			}

			if (m_Win->Close())
				m_Running = false;
		}

		void Application::OnUpdate(Utils::Timestep &ts)
		{
			m_LayerStack.OnUpdate(ts);
		}

		void Application::OnRender(float alpha)
		{
			m_LayerStack.OnRender(alpha);
		}

		void Application::OnTick()
		{
			m_Win->OnTick();

			m_LayerStack.OnTick();
		}

		void Application::OnSuspended()
		{
			m_LayerStack.OnSuspended();
		}

		Application &Application::Get() 
		{
			mc_assert(s_Instance, "application instance is null pointer\n");
			return *s_Instance;
		}
	}
}