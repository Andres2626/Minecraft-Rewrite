#pragma once

#include "common.h"
#include "Window.h"
#include "Layer/DefaultLayer.h"
#include "Utils/Util.h"
#include "Utils/Timer.h"

namespace MC {
	namespace App {

		class Application {
		private:
			static Application* m_Instance;
		protected:
			Window* m_Win;
		private:
			bool m_Running, m_Suspended;
			Utils::Timer* m_Timer;
			int m_FPS, m_UPS;
			float m_FrameTime;

			std::string m_Name;
			WindowProperties m_Pr;

			std::vector<MC::Layer::DefaultLayer*> m_LayerStack;
		public:
			Application(const std::string& name, const WindowProperties& pr);
			~Application();
		public:
			virtual void Init();
		public:
			void PushLayer(Layer::DefaultLayer* layer);
			Layer::DefaultLayer* PopLayer();
			Layer::DefaultLayer* PopLayer(Layer::DefaultLayer* layer);
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
			void OnUpdate(Utils::Timestep& ts);
			void OnEvent(Events::Event& ev);
			void OnRender();
			void OnTick();
			void OnSuspended();
		public:
			inline static Application& GetInstance() { return *m_Instance; };
		public:
			inline Window& GetWindow() { return *this->m_Win; };
		};

	}
}