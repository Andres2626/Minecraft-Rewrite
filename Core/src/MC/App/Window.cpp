#include <glad/glad.h>
#include "Window.h"

#include "Input.h"
#include "Events/Event.h"
#include "Log/Log.h"

namespace MC 
{
	namespace App 
	{

		/* GLFW Error handler */
		void ErrorCallback(int error, const char* msg)
		{
			MC_FATAL << "GLFW Error " << error << ":" << msg;
		}

		Window::Window(const rd_str_t& title, const WindowProperties& properties)
			: m_Title(title), m_Pr(properties), m_Init(false)
		{
			/* Create window */
			m_Init = Create();
			if (!m_Init)
				Finish();
		}

		Window::~Window()
		{
			glfwTerminate();
		}

		bool Window::Create()
		{
			glfwSetErrorCallback(ErrorCallback);

			if (!glfwInit()) {
				MC_FATAL << "GLFW not initialized";
				return false;
			}

			/* Initailze context */
			if (m_Pr.context.ver_major)
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Pr.context.ver_major);

			if (m_Pr.context.ver_minor)
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Pr.context.ver_minor);

			if (m_Pr.context.profile)
				glfwWindowHint(GLFW_OPENGL_PROFILE, m_Pr.context.profile);

			if (m_Pr.context.compat)
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			/* Create window via GLFW */
			m_Win = glfwCreateWindow(m_Pr.x, m_Pr.y, m_Title.c_str(), 0, 0);
			if (!m_Win) {
				MC_FATAL << "Error initializing window (" << m_Pr.x << "x" << m_Pr.y << "," << "title:\"" << m_Title << "\"" << ")";
				return false;
			}

			glfwMakeContextCurrent(m_Win);

			/* Initialze event system */
			gleqInit();
			gleqTrackWindow(m_Win);

			bool glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			if (!glad) {
				MC_FATAL << "GL context not initialized";
				return false;
			}

			if (!m_Pr.cursor.enable)
				glfwSetInputMode(m_Win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			MC_WARN << "OpenGL Version: " << glGetString(GL_VERSION);
			MC_WARN << "GPU vendor: " << glGetString(GL_VENDOR);
			MC_WARN << "GPU name: " << glGetString(GL_RENDERER);

			return true;
		}

		void Window::Finish()
		{
			glfwTerminate();
		}

		bool Window::Close()
		{
			return glfwWindowShouldClose(m_Win);
		}

		void Window::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void Window::Update()
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Win);
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

			glfwSetWindowIcon(m_Win, 1, image);
		}

	}
}