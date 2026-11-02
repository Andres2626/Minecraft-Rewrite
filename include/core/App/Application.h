#pragma once
#include "common.h"
#include "Window.h"

#include "Layers/Stack.h"
#include "Utils/Timer.h"

namespace MC 
{
	namespace App 
	{
        struct ApplicationContext
        {
            Window *Win;
            WindowProperties WinProps;
            Utils::Timestep TimerStep;
            const char *name;
            float TimerAcum;
            float UpdateTimer;
            float UpdateTick;
            float FrameTime;
            bool Running;
            bool Suspended;
            int FPS;
            int UPS;
        };

        /**
         * \brief Main application of MC engine.
         * 
         * \note Use Application class as the base class for the game.
         *
         * @deprecated Application as a class inherited from Layer.
         */
		class MC_API Application : public NonCopyable, public NonMovable
		{
        protected:
            ApplicationContext ctx;	
		public:
             /**
              * \brief Application constructor.
              * 
              * \param name Application name.
              * \param pr Window properties.
              *
              * \see WindowProperties
              */
			Application(const char *name, const WindowProperties &pr);
			virtual ~Application();
		public:
            /**
             * \brief Initialize application.
             * 
             * Initialize all MC engine subsystems.
             * 
             * \note This function should not be called directly, as \ref Start() 
             * calls it. It is public so that the Game class inherits Init() from 
             * Application.
             *
             * \see WindowProperties
             */
			virtual ErrorType Init();
		public:
            /**
             * \brief Inserts a new main game layer into the layer stack. 
             *
             * \param layer Game layer.
             */
			void PushLayer(Layers::Layer *layer);
		public:
            /**
             * \brief Starts the application cycle
             * 
             * \note This controls the internal state of the application cycle.
             */
			void Start();
            
            /**
             * \brief Suspend the application cycle
             *
             * \note This controls the internal state of the application cycle.
             */
			void Suspend();
            
            /**
             * \brief Resume the application cycle
             * 
             * \note This controls the internal state of the application cycle.
             */
			void Resume();
            
            /**
             * \brief Stop the application cycle
             * 
             * \note This controls the internal state of the application cycle.
             */
			void Stop();
		private:
			void Run();
			void Shutdown();
        public:
            void Frame();
        private:
			void OnUpdate(Utils::Timestep &ts);
			void OnRender(float alpha);
			void OnTick();
			void OnSuspended();      
		public:
            /**
             * \brief Get current application instance.
             *
             * \pre The application must be initialized.
             * 
             * \return Pointer of current application instance.
             */
			static Application &Get();

            /**
             * \brief Get application context
             *
             * \pre The application must be initialized.
             * 
             * \return Struct of application context.
             */
            static ApplicationContext &GetContext();
		};

	}
}