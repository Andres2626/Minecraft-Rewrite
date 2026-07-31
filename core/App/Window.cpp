#include "Graphics/GL/GL.h"
#include "App/Window.h"

#include "App/Input.h"
#include "Events/EventManager.h"
#include "Events/WindowEvent.h"
#include "Events/FrameBufferEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MonitorEvent.h"
#include "Events/JoystickEvent.h"
#include "Graphics/Renderer.h"
#include "Graphics/GL/GLError.h"
#include "Graphics/GL/GLContext.h"

#define MC_LOG_PREFIX "Window"
#include "Log/Log.h"

#ifdef MC_PLATFORM_WEB
#include <emscripten/html5.h>
#endif

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
			: m_Title(title), m_Pr(properties), m_Init(false), m_MouseFirst(true), m_MouseLast({0.0f, 0.0f})
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
            
            /* Create window via GLFW */
			GLFWmonitor* monitor = nullptr;

			int width = m_Pr.x;
			int height = m_Pr.y;

#ifdef MC_PLATFORM_WEB
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
			
#else /* !MC_PLATFORM_WEB */
            if (m_Pr.context.profile)
				glfwWindowHint(GLFW_OPENGL_PROFILE, m_Pr.context.profile);

			if (m_Pr.context.compat)
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            
			if (m_Pr.context.ver_major)
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Pr.context.ver_major);

			if (m_Pr.context.ver_minor)
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Pr.context.ver_minor);
        
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
#endif /* MC_PLATFORM_WEB */
			m_Win = glfwCreateWindow(width, height, m_Title, monitor, 0);
			if (!m_Win) {
				mc_fatal("error creating window: x={} y={} title={}", width, height, m_Title);
				err.SetError(ErrorType::WindowBuild);
				return false;
			}
            
            glfwSetWindowUserPointer(m_Win, this);

			if (!Graphics::GL::Context::Init((void*)m_Win)) {
				mc_fatal("error creating window context");
				err.SetError(ErrorType::ContextInit);
				return false;
			}

			glfwSetWindowPosCallback(m_Win, [](GLFWwindow* native, int x, int y) {
				QUEUE_EVENT(Events::WindowMovedEvent, x, y);
				});

			glfwSetWindowSizeCallback(m_Win, [](GLFWwindow* native, int width, int height) {
				QUEUE_EVENT(Events::WindowResizeEvent, width, height);
				});

            glfwSetWindowFocusCallback(m_Win, [](GLFWwindow* native, int focus) {
				QUEUE_EVENT(Events::WindowFocusEvent, focus);
				});

			glfwSetWindowContentScaleCallback(m_Win, [](GLFWwindow* native, float x, float y) {
				QUEUE_EVENT(Events::WindowScaledEvent, x, y);
				});
				
			glfwSetFramebufferSizeCallback(m_Win, [](GLFWwindow* native, int width, int height) {
				QUEUE_EVENT(Events::FrameBufferResizeEvent, width, height);
				});

			glfwSetMouseButtonCallback(m_Win, [](GLFWwindow* native, int button, int action, int mods) {
#ifdef MC_PLATFORM_WEB
				bool locked = false;
				Window* win = static_cast<Window*>(glfwGetWindowUserPointer(native));
				const WindowProperties& props = win->GetProps();
                EmscriptenPointerlockChangeEvent ev;

                if (emscripten_get_pointerlock_status(&ev) == EMSCRIPTEN_RESULT_SUCCESS)
                    locked = ev.isActive;
                
                if (!locked && !props.cursor.enable && action == GLFW_PRESS) {
                    emscripten_request_pointerlock("#canvas", EM_TRUE);
                    return; /* Do not process the first event; it is used to disable the cursor. */
                }
#endif
                
				QUEUE_EVENT(Events::MouseButtonEvent, button, (Events::ButtonAction)action, mods);
				});

			glfwSetCursorEnterCallback(m_Win, [](GLFWwindow* native, int entered) {
				QUEUE_EVENT(Events::CursorEnterEvent, entered);
				});

			glfwSetScrollCallback(m_Win, [](GLFWwindow* native, double x, double y) {
				QUEUE_EVENT(Events::MouseScrollEvent, x, y);
				});

			glfwSetKeyCallback(m_Win, [](GLFWwindow* native, int key, int scancode, int action, int mods) {
				QUEUE_EVENT(Events::KeyboardButtonEvent, key, scancode, (Events::ButtonAction)action, mods);
			});

			glfwSetJoystickCallback([](int id, int event) {
				QUEUE_EVENT(Events::JoystickEvent, id, event);
				});
                
#ifndef MC_PLATFORM_WEB
            glfwSetCursorPosCallback(m_Win, [](GLFWwindow* native, double x, double y) {
				Window* win = static_cast<Window*>(glfwGetWindowUserPointer(native));
                if (win->GetMouseFirst()) {
                    win->SetMouseLast({x, y});
                    win->SetMouseFirst(false);
                    return;
                }
                
                Math::vec2 last = win->GetMouseLast();
                double dx = x - last.x;
                double dy = last.y - y;

                win->SetMouseLast({x, y});

                QUEUE_EVENT(Events::CursorMotionEvent, dx, dy);
				});

			glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) {
				QUEUE_EVENT(Events::MonitorEvent, event);
				});
                
            glfwSetWindowCloseCallback(m_Win, [](GLFWwindow* native) {
				QUEUE_EVENT(Events::WindowCloseEvent);
				});

			glfwSetWindowRefreshCallback(m_Win, [](GLFWwindow* native) {
				QUEUE_EVENT(Events::WindowRefreshEvent);
				});
                
            glfwSetWindowIconifyCallback(m_Win, [](GLFWwindow* native, int icon) {
				QUEUE_EVENT(Events::WindowIconifyEvent, icon);
				});

			if (!m_Pr.cursor.enable)
				glfwSetInputMode(m_Win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(m_Win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                
#else /* MC_PLATFORM_WEB */
            emscripten_set_mousemove_callback("#canvas", nullptr, EM_TRUE,
            [](int eventType, const EmscriptenMouseEvent* e, void*) -> EM_BOOL
            {
                QUEUE_EVENT(Events::CursorMotionEvent, (float)e->movementX, (float)-e->movementY);
                return EM_TRUE;
            });
#endif /* !MC_PLATFORM_WEB */

			mc_info("Window: x={} y={} title={} cursor={} profile={} glver={} {} compat={}", 
					m_Pr.x, 
					m_Pr.y, 
					m_Title,
					m_Pr.cursor.enable,
					m_Pr.context.profile,
					m_Pr.context.ver_major,
					m_Pr.context.ver_minor,
					m_Pr.context.compat);
            
            Graphics::Renderer::Init();
            
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
#ifndef MC_PLATFORM_WEB         
			glfwSwapBuffers(m_Win);
#endif
		}

		void Window::SetIcon(const Graphics::Image &img)
		{
#ifndef MC_PLATFORM_WEB
			GLFWimage image[1];
			image[0].width = img.x;
			image[0].height = img.y;
			image[0].pixels = img.pixels;

			glfwSetWindowIcon(m_Win, 1, image);
#endif /* MC_PLATFORM_WEB */
		}

		bool Window::SetFullScreen(bool fullscreen)
		{
#ifndef MC_PLATFORM_WEB
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
#endif /* MC_PLATFORM_WEB */
			return true;
		}
        
        void Window::SetMouseFirst(bool first)
        {
            m_MouseFirst = first;
        }
        
        void Window::SetMouseLast(const Math::vec2& last)
        {
            m_MouseLast = last;
        }

	}
}