#pragma once
#include "common.h"
#include "Window.h"

#include "Layer/Stack.h"
#include "Log/Log.h"
#include "Utils/Timer.h"

namespace MC {
	namespace App {

		class MC_API Application : public Layer::ILayer {
		protected:
			Window* m_Win;
		private:
			bool m_Running;
			bool m_Suspended;
			int m_FPS;
			int m_UPS;
			float m_FrameTime;
			rd_str_t m_Name;
			WindowProperties m_Pr;
		private:
			Utils::Timer* m_Timer;
			Layer::Stack m_LayerStack;
		public:
			Application(const rd_str_t& name, const WindowProperties& pr);
			~Application();
		public:
			virtual void Init();
		public:
			void PushLayer(Layer::ILayer* layer);
			Layer::ILayer* PopLayer();
			Layer::ILayer* PopLayer(Layer::ILayer* layer);
		public:
			void Start();
			void Suspend();
			void Resume();
			void Stop();
		public:
			inline int GetFPS() { return this->m_FPS; }
			inline int GetUPS() { return this->m_UPS; }
			inline float GetFrameTime() { return this->m_FrameTime; }
		private:
			void Run();
			void OnUpdate(Utils::Timestep& ts) override;
			void OnEvent(Events::Event& ev) override;
			void OnRender() override;
			void OnTick() override;
			void OnSuspended() override;
		public:
			static Application& GetInstance();
		public:
			inline Window& GetWindow() { return *this->m_Win; };
		};

	}
}