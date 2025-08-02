#pragma once
#include "common.h"
#include "Graphics/Image.h"
#include "Events/Event.h"

#include <GLFW/glfw3.h>

/* from GLFW.h */

#define CONTEXT_ANY_PROFILE 0 /* supported in any GL version */
#define CONTEXT_CORE_PROFILE 0x00032001 /* supported in GL versions >=3.2 */
#define CONTEXT_COMPAT_PROFILE 0x00032002 /* supported in GL versions >=3.2 */

namespace MC {
	namespace App {

		/* This struct is used when window class is initialized.
		have some properties of window*/
		struct RD_API WindowProperties {
			int x; /* width */
			int y; /* height */

			/* Cursor properties */
			struct {
				bool enable;
			} cursor;

			/* OpenGL context */
			struct {
				int profile;
				int ver_major;
				int ver_minor;
				bool compat;
			} context;
		};

		class RD_API Window {
		public:
			bool IsInititialized;
		private:
			WindowProperties w_pr;
			const char* m_Title;
			GLFWwindow* internal_window; /* OpenGL window */
		public:
			/* Initializes and create window */
			Window(const char* title, const WindowProperties& properties);
			~Window();
		private:
			/* returns FALSE in case of error */
			bool Create();
		public:
			bool Close();
		public:
			void Clear();
			void Update();
			void Finish();
		public:
			int GetEvent(MC::Events::Event& ev);
			void FreeEvent(MC::Events::Event& ev);
		public:
			void SetIcon(const Graphics::Image& img);
		public:
			inline WindowProperties& GetProps() { return this->w_pr; }
			inline GLFWwindow* GetWindow() { return this->internal_window; };
		};

	}
}