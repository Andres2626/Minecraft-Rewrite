#include "mc.h"

#include "App/Application.h"

#include "App/Input.h"

#include "Events/EventManager.h"
#include "Utils/Util.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

#ifdef MC_PLATFORM_WEB
#include <emscripten.h>

static void WebLoop()
{
    auto &app = MC::App::Application::Get();
    if (!app.GetContext().Running)
        return;
    app.Frame();
}

#endif /* MC_PLATFORM_WEB */

static void ErrorCallback(int error, const char* msg)
{
	mc_error("glfw error {}: {}\n", error, msg);
}

namespace MC 
{
	namespace App 
	{
		static Application* s_Instance = nullptr;
		static Utils::Timer s_Timer;
		static Layers::Stack s_LayerStack;

		Application::Application(const char *name, const WindowProperties &pr)
		{
			ctx.FPS = 0;
			ctx.UPS = 0;
			ctx.FrameTime = 0.0f;
			ctx.name = name;
			ctx.WinProps = pr;
			ctx.Running = false;
			ctx.Suspended = false;
			s_Instance = this;
		} 

		Application::~Application()
		{
			if (s_Instance)
				s_Instance = nullptr;
		}

		ErrorType Application::Init()
		{
			Log::Init(MC_LOG_STDOUT | MC_LOG_FILE, level_enum::trace);
			mc_info("Minecraft rewrite engine version {}", MC_VERSION_STRING);

			glfwSetErrorCallback(ErrorCallback);

			if (!glfwInit()) {
				mc_error("error initializing window library");
				return ErrorType::WindowLibrary;
			}

			ctx.Win = new Window(ctx.name, ctx.WinProps);

			ErrorType err = ctx.Win->Create();
			if (err != ErrorType::NoError) {
                ctx.Win->Finish();
				mc_error("error building window");
				return err;
			}

			Input::Init();

			return ErrorType::NoError;
		}

		void Application::PushLayer(Layers::Layer *layer)
		{
			s_LayerStack.PushLayer(layer);
		}

		void Application::Start()
		{
			ErrorType err;
			err = Init();
			if (err != ErrorType::NoError) {
				mc_fatal("engine error {}", (u32t)err);
				goto app_end;
			}

			
			if (!s_LayerStack.Init()) {
				mc_error("error initializing layer stack");
				goto app_end;
			}

			ctx.Running = true;
			ctx.Suspended = false;
			
			Run();
		app_end:
			Shutdown();
			return;
		}

		void Application::Suspend()
		{
			ctx.Suspended = true;
		}

		void Application::Resume()
		{
			ctx.Suspended = false;
		}

		void Application::Stop()
		{
			ctx.Running = false;
		}

		void Application::Run()
		{
			ctx.TimerAcum = 0.0f;
			ctx.UpdateTimer = s_Timer.ElapsedMillis();
			ctx.UpdateTick = 1000.0f / 20.0f;
			ctx.TimerStep = Utils::Timestep(ctx.UpdateTimer);
            
#ifndef MC_PLATFORM_WEB
			while (ctx.Running)
				Frame();
#else
            emscripten_set_main_loop(WebLoop, 0, true);
#endif /* !MC_PLATFORM_WEB */

            Stop();
		}

		void Application::Shutdown()
		{
#ifdef MC_PLATFORM_WEB
			emscripten_cancel_main_loop();
#endif /* MC_PLATFORM_WEB */

			s_LayerStack.Finish();
			Input::Finish();

			if (ctx.Win) {
				delete ctx.Win;
				ctx.Win = nullptr;
			}

			glfwTerminate();
			Log::Finish();
		}

		void Application::Frame()
		{
			Utils::Timer frametime;
            
            if (ctx.Suspended) {
                OnSuspended();
                return;
            }
            
			DISPATCH_EVENTS();

			ctx.Win->Clear();
			float now = s_Timer.ElapsedMillis();
			while (now - ctx.UpdateTimer >= ctx.UpdateTick) {
				ctx.TimerStep.Update(now);
				OnUpdate(ctx.TimerStep);
				ctx.UPS++;
				ctx.UpdateTimer += ctx.UpdateTick;
			}

			float alpha = (now - ctx.UpdateTimer) / ctx.UpdateTick;
			OnRender(alpha);

			ctx.Win->Update();
			ctx.FPS++;
			ctx.FrameTime = frametime.ElapsedMillis();

			if (now - ctx.TimerAcum > 1000.0f) {
				ctx.TimerAcum += 1000.0f;
				OnTick();
				ctx.FPS = 0;
				ctx.UPS = 0;
			}

			if (ctx.Win->IsClosed())
				ctx.Running = false;
		}

		void Application::OnUpdate(Utils::Timestep &ts)
		{
			s_LayerStack.OnUpdate(ts);
		}

		void Application::OnRender(float alpha)
		{
			s_LayerStack.OnRender(alpha);
		}

		void Application::OnTick()
		{
			ctx.Win->OnTick();

			s_LayerStack.OnTick();
		}

		void Application::OnSuspended()
		{
			s_LayerStack.OnSuspended();
		}

		Application &Application::Get()
		{
			mc_assert(s_Instance, "application instance is null pointer\n");
			return *s_Instance;
		}

		ApplicationContext &Application::GetContext()
		{
			return Application::Get().ctx;
		}
	}
}