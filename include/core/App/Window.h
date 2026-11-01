#pragma once
#include "common.h"

#include "Graphics/Image.h"
#include "Utils/Error.h"
#include "Utils/NonCopyable.h"
#include "Utils/NonMovable.h"
#include "Math/Math.h"

namespace MC 
{
	namespace App 
	{
        /**
         * \brief Properties used to create a window.
         */
		struct WindowProperties 
		{
            /**
             * \brief Window width.
             */
			int width;
            
            /**
             * \brief Window height.
             */
			int height;
            
            /**
             * \brief Window fullscreen state.
             */
			bool fullscreen;
		};

		class MC_API Window : public NonCopyable, public NonMovable
		{
		private:
			const char *m_Title;
			WindowProperties m_Properties;
			void *m_Handle;
		public:
			/**
			  * \brief Window constructor.
			  *
			  * \param name Window title.
			  * \param properties Window properties. 
			  * 
			  * \pre Engine must be initialized.
			  * 
			  * \see WindowProperties
			  */
			Window(const char *title, const WindowProperties &properties);
			~Window();		
		public:
			/**
			  * \brief Create window and initialize graphic context.
			  *
			  * \return 0 on sucess, >0 on error.
			  */
			ErrorType Create();
			/**
			  * \brief Finish GLFW window.
			  */
			void Finish();	
		public:
			/**
			  * \brief Clear buffers.
			  */
			void Clear();

			/**
			  * \brief Poll events and swap buffers.
			  */
			void Update();

			/**
			  * \brief Get if window is closed.
			  */
			bool IsClosed();
		public:
			void OnTick();
		public:
			/**
			  * \brief Set window icon from image.
			  * 
			  * \note This doesn't work on web platform
			  */
			void SetIcon(const Graphics::Image &img);

			/**
			  * \brief Set window fullscreen.
			  *
			  * \note This doesn't work on web platform
			  */
			bool SetFullScreen(bool fullscreen);
		public:
			/**
			  * \brief Get window properties.
			  */
			WindowProperties &GetProperties();

			/**
			  * \brief Get GLFW window.
			  */
			void *GetHandle();
		};

	}
}