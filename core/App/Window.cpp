#include <gfx/glad.h>
#include "App/Window.h"

#include "App/Input.h"
#include "Events/Event.h"
#include "Graphics/Renderer.h"
#include "Graphics/GL/GLError.h"
#include "Graphics/GL/GLContext.h"

#define MC_LOG_PREFIX "Window"
#include "Log/Log.h"

namespace MC 
{
	namespace App 
	{
		/* GLFW Error handler */
		void ErrorCallback(int error, const char *msg)
		{
			mc_fatal("glfw error {}: {}\n", error, msg);
		}

		Window::Window(const char *title, const WindowProperties &properties)
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
				err.SetError(ErrorType::WindowLibrary);
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
			GLFWmonitor* monitor = nullptr;

			int width = m_Pr.x;
			int height = m_Pr.y;

			if (m_Pr.fullscreen)
			{
				monitor = glfwGetPrimaryMonitor();
				if (!monitor) {
					mc_fatal("error obtaining window monitor");
					err.SetError(ErrorType::WindowBuild);
					return false;
				}

				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				if (!mode) {
					mc_fatal("error obtaining video mode");
					err.SetError(ErrorType::WindowBuild);
					return false;
				}

				width = mode->width;
				height = mode->height;
			}

			m_Win = glfwCreateWindow(width, height, m_Title, monitor, 0);
			if (!m_Win) {
				mc_fatal("error creating window: x={} y={} title={}", width, height, m_Title);
				err.SetError(ErrorType::WindowBuild);
				return false;
			}

			if (!Graphics::GL::Context::Init((void*)m_Win)) {
				mc_fatal("error creating window context");
				err.SetError(ErrorType::ContextInit);
				return false;
			}

			/* Initialze event system */
			gleqInit();
			gleqTrackWindow(m_Win);

			if (!m_Pr.cursor.enable)
				glfwSetInputMode(m_Win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			mc_info("Window: x={} y={} title={} cursor={} profile={} glver={} {} compat={}", 
					m_Pr.x, 
					m_Pr.y, 
					m_Title,
					m_Pr.cursor.enable,
					m_Pr.context.profile,
					m_Pr.context.ver_major,
					m_Pr.context.ver_minor,
					m_Pr.context.compat);

			mc_warn("graphic driver version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
			mc_warn("driver vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
			mc_warn("driver name: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

			return true;
		}

		void Window::Finish()
		{
			glfwTerminate();
		}

		void Window::OnTick()
		{
			u32t err = Graphics::GL::Error::GetError();
			if (err != GL_NO_ERROR)
				mc_error("OpenGL error {}: {}", err, Graphics::GL::Error::GetErrorStr(err));
		}

		bool Window::Close()
		{
			return glfwWindowShouldClose(m_Win);
		}

		void Window::Clear()
		{
			Graphics::Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void Window::Update()
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Win);
		}

		int Window::GetEvent(Events::Event &ev)
		{
			return gleqNextEvent(&ev);
		}

		void Window::FreeEvent(Events::Event &ev)
		{
			gleqFreeEvent(&ev);
		}

		void Window::SetIcon(const Graphics::Image &img)
		{
			GLFWimage image[1];
			image[0].width = img.x;
			image[0].height = img.y;
			image[0].pixels = img.pixels;

			glfwSetWindowIcon(m_Win, 1, image);
		}

		bool Window::SetFullScreen(bool fullscreen)
		{
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			if (!monitor)
				return false;

			const GLFWvidmode *mode = glfwGetVideoMode(monitor);
			if (!mode)
				return false;

			if (fullscreen)
				glfwSetWindowMonitor(m_Win, monitor, 0, 0, mode->width, mode->height, 0);
			else
				glfwSetWindowMonitor(m_Win, nullptr, 100, 100, m_Pr.x, m_Pr.y, 0);

			return true;
		}

	}
}