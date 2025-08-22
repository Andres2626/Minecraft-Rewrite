#pragma once
#include "common.h"
#include "Window.h"

#include "Layers/Stack.h"
#include "Log/Log.h"
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
			bool m_Running;
			bool m_Suspended;
			int m_FPS;
			int m_UPS;
			float m_FrameTime;
			mc_str m_Name;
		private:
			std::unique_ptr<Utils::Timer> m_Timer;
			Layers::Stack m_LayerStack;
		public:
			Application(const mc_str& name, const WindowProperties& pr);
			~Application();
		public:
			virtual void Init();
		public:
			void PushLayer(Layers::Layer* layer);
		public:
			void Start();
			void Suspend();
			void Resume();
			void Stop();
		private:
			void Run();
			void OnUpdate(Utils::Timestep& ts) override;
			void OnEvent(Events::Event& ev) override;
			void OnRender() override;
			void OnTick() override;
			void OnSuspended() override;
		public:
			inline int GetFPS() { return m_FPS; }
			inline int GetUPS() { return m_UPS; }
			inline float GetFrameTime() { return m_FrameTime; }
		public:
			static Application& Get();
			static inline WindowProperties& GetProperties() { return Get().m_Pr; };
			inline Window& GetWindow() { return *m_Win; };
		};

	}
}