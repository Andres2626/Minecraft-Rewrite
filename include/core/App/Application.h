#pragma once
#include "common.h"
#include "Window.h"

#include "Layers/Stack.h"
#include "Utils/Timer.h"

namespace MC 
{
	namespace App 
	{
		class MC_API Application : public Layers::Layer 
		{
		protected:
			std::unique_ptr<Window> m_Win;
			WindowProperties m_Pr;
		private:
			float m_TimeAcumulator;
			float m_UpdateTimer;
			float m_UpdateTick;
			Utils::Timestep m_Step;
			bool m_Running;
			bool m_Suspended;
			int m_FPS;
			int m_UPS;
			float m_FrameTime;
			const char *m_Name;
		private:
			Utils::Timer m_Timer;
			Layers::Stack m_LayerStack;
		public:
			Application(const char *name, const WindowProperties &pr);
			~Application();
		public:
			virtual bool Init() override;
		public:
			void PushLayer(Layers::Layer *layer);
		public:
			void Start();
			void Suspend();
			void Resume();
			void Stop();
            void Frame();
		private:
			void Run();
			void Shutdown();
			void OnUpdate(Utils::Timestep &ts) override;
			void OnRender(float alpha) override;
			void OnTick() override;
			void OnSuspended() override;
		public:
			inline int GetFPS() { return m_FPS; }
			inline int GetUPS() { return m_UPS; }
			inline float GetFrameTime() { return m_FrameTime; }
		public:
			static Application &Get();
			static inline WindowProperties &GetProperties() { return Get().m_Pr; };
			inline Window &GetWindow() { return *m_Win; };
            inline bool IsRunning() { return m_Running; }
		};

	}
}