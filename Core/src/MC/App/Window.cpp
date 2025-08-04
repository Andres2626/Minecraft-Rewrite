#include <glad/glad.h>
#include "Window.h"

#include "Input.h"
#include "Events/Event.h"
#include "Log/Log.h"

namespace MC {
	namespace App {

		/* GLFW Error handler */
		void ErrorCallback(int error, const char* msg) 
		{
			RD_FATAL << "GLFW Error" << error << ":"  << msg;
		}

		Window::Window(const char* title, const WindowProperties& properties)
			: m_Title(title), w_pr(properties), IsInititialized(false)
		{
			/* Create window */
			this->IsInititialized = Create();
			if (!this->IsInititialized)
				this->Finish();
		}

		Window::~Window() 
		{
			glfwTerminate();
		}

		bool Window::Create() 
		{
			glfwSetErrorCallback(ErrorCallback);

			if (!glfwInit()) {
				printf("GLFW Error. Init()\n");
				return false;
			}

			/* Initailze context */
			if (this->w_pr.context.ver_major)
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->w_pr.context.ver_major);

			if (this->w_pr.context.ver_minor)
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->w_pr.context.ver_minor);

			if (this->w_pr.context.profile)
				glfwWindowHint(GLFW_OPENGL_PROFILE, this->w_pr.context.profile);

			if (this->w_pr.context.compat)
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			/* Create window via GLFW */
			this->internal_window = glfwCreateWindow(this->w_pr.x, this->w_pr.y, this->m_Title, 0, 0);
			RD_FATAL_CHK(this->internal_window) << "Error initializing window";
			glfwMakeContextCurrent(this->internal_window);

			/* Initialze event system */
			gleqInit();
			gleqTrackWindow(this->internal_window);

			RD_FATAL_CHK(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) << "Error initializing OpenGL";

			if (!this->w_pr.cursor.enable)
				glfwSetInputMode(this->internal_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			RD_WARN << "OpenGL Version: " <<  glGetString(GL_VERSION);
			RD_WARN << "GPU type: " << glGetString(GL_VENDOR);
			RD_WARN << "GPU name: " << glGetString(GL_RENDERER);

			return true;
		}

		void Window::Finish() 
		{
			glfwTerminate();
		}

		bool Window::Close() 
		{
			return glfwWindowShouldClose(this->internal_window);
		}

		void Window::Clear() 
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void Window::Update() 
		{
			glfwPollEvents();
			glfwSwapBuffers(this->internal_window);
		}

		int Window::GetEvent(MC::Events::Event& ev) 
		{
			return gleqNextEvent(&ev);
		}

		void Window::FreeEvent(MC::Events::Event& ev) 
		{
			gleqFreeEvent(&ev);
		}

		void Window::SetIcon(const Graphics::Image& img)
		{
			GLFWimage image[1];
			image[0].width = img.x;
			image[0].height = img.y;
			image[0].pixels = img.pixels;

			glfwSetWindowIcon(this->internal_window, 1, image);
		}
	}
}