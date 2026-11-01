#include "mc.h"

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

#include <GLFW/glfw3.h>

#define MC_LOG_PREFIX "Window"
#include "Log/Log.h"

#ifdef MC_PLATFORM_WEB
#include <emscripten/html5.h>
#endif

namespace MC 
{
	namespace App 
	{

		Window::Window(const char *title, const WindowProperties &properties)
			: m_Title(title), m_Properties(properties)
		{

		}

		Window::~Window()
		{
			Finish();
		}

		ErrorType Window::Create()
		{   
            /* Create window via GLFW */
			GLFWmonitor* monitor = nullptr;

			int width = m_Properties.width;
			int height = m_Properties.height;

			if (m_Properties.fullscreen)
			{
				monitor = glfwGetPrimaryMonitor();
				if (!monitor) {
					mc_error("error obtaining window monitor");
					return ErrorType::WindowBuild;
				}

				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				if (!mode) {
					mc_error("error obtaining video mode");
					return ErrorType::WindowBuild;
				}

				width = mode->width;
				height = mode->height;
			}

#ifdef MC_PLATFORM_WEB
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif /* MC_PLATFORM_WEB */

			m_Handle = (void*)glfwCreateWindow(width, height, m_Title, monitor, 0);
			GLFWwindow *win = (GLFWwindow*)m_Handle;
			if (!m_Handle) {
				mc_error("error creating window");
				return ErrorType::WindowBuild;
			}
            
            glfwSetWindowUserPointer(win, this);

			if (!Graphics::GL::Context::Init(m_Handle)) {
				mc_error("error creating window context");
				return ErrorType::ContextInit;
			}

			glfwSetWindowPosCallback(win, [](GLFWwindow* native, int x, int y) {
				QUEUE_EVENT(Events::WindowMovedEvent, x, y);
				});

			glfwSetWindowSizeCallback(win, [](GLFWwindow* native, int width, int height) {
				QUEUE_EVENT(Events::WindowResizeEvent, width, height);
				});

            glfwSetWindowFocusCallback(win, [](GLFWwindow* native, int focus) {
				QUEUE_EVENT(Events::WindowFocusEvent, focus);
				});

			glfwSetWindowContentScaleCallback(win, [](GLFWwindow* native, float x, float y) {
				QUEUE_EVENT(Events::WindowScaledEvent, x, y);
				});
				
			glfwSetFramebufferSizeCallback(win, [](GLFWwindow* native, int width, int height) {
				QUEUE_EVENT(Events::FrameBufferResizeEvent, width, height);
				});

			glfwSetMouseButtonCallback(win, [](GLFWwindow* native, int button, int action, int mods) {
#ifdef MC_PLATFORM_WEB
				bool locked = false;
				Window *win = static_cast<Window*>(glfwGetWindowUserPointer(native));
				const WindowProperties& props = win->GetProperties();
                EmscriptenPointerlockChangeEvent ev;

                if (emscripten_get_pointerlock_status(&ev) == EMSCRIPTEN_RESULT_SUCCESS)
                    locked = ev.isActive;
                
                if (!locked && action == GLFW_PRESS) {
                    emscripten_request_pointerlock("#canvas", EM_TRUE);
                    return; /* Do not process the first event; it is used to disable the cursor. */
                }
#endif
                
				QUEUE_EVENT(Events::MouseButtonEvent, button, (Events::ButtonAction)action, mods);
				});

			glfwSetCursorEnterCallback(win, [](GLFWwindow* native, int entered) {
				QUEUE_EVENT(Events::CursorEnterEvent, entered);
				});

			glfwSetScrollCallback(win, [](GLFWwindow* native, double x, double y) {
				QUEUE_EVENT(Events::MouseScrollEvent, x, y);
				});

			glfwSetKeyCallback(win, [](GLFWwindow* native, int key, int scancode, int action, int mods) {
				QUEUE_EVENT(Events::KeyboardButtonEvent, key, scancode, (Events::ButtonAction)action, mods);
			});

			glfwSetJoystickCallback([](int id, int event) {
				QUEUE_EVENT(Events::JoystickEvent, id, event);
				});
                
#ifndef MC_PLATFORM_WEB
            glfwSetCursorPosCallback(win, [](GLFWwindow* native, double x, double y) {
                
                if (Input::GetMouseFirst()) {
                    Input::SetMouseLast({x, y});
                    Input::SetMouseFirst(false);
                    return;
                }
                
                Math::vec2 last = Input::GetMouseLast();
                double dx = x - last.x;
                double dy = last.y - y;

                Input::SetMouseLast({x, y});

                QUEUE_EVENT(Events::CursorMotionEvent, dx, dy);
				});

			glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) {
				QUEUE_EVENT(Events::MonitorEvent, event);
				});
                
            glfwSetWindowCloseCallback(win, [](GLFWwindow* native) {
				QUEUE_EVENT(Events::WindowCloseEvent);
				});

			glfwSetWindowRefreshCallback(win, [](GLFWwindow* native) {
				QUEUE_EVENT(Events::WindowRefreshEvent);
				});
                
            glfwSetWindowIconifyCallback(win, [](GLFWwindow* native, int icon) {
				QUEUE_EVENT(Events::WindowIconifyEvent, icon);
				});

#else /* MC_PLATFORM_WEB */
            emscripten_set_mousemove_callback("#canvas", nullptr, EM_TRUE,
            [](int eventType, const EmscriptenMouseEvent* e, void*) -> EM_BOOL
            {
                QUEUE_EVENT(Events::CursorMotionEvent, (float)e->movementX, (float)-e->movementY);
                return EM_TRUE;
            });
#endif /* !MC_PLATFORM_WEB */

			mc_info("Window: width={} height={} title={}", 
					m_Properties.width,
					m_Properties.height,
					m_Title);
            
            Graphics::Renderer::Init();
            
			return ErrorType::NoError;
		}

		void Window::Finish()
		{
			if (m_Handle) {
				glfwDestroyWindow((GLFWwindow*)m_Handle);
				m_Handle = nullptr;
			}
		}

		void Window::OnTick()
		{
			u32t err = Graphics::GL::Error::GetError();
			if (err != GL_NO_ERROR)
				mc_error("OpenGL error {}: {}", err, Graphics::GL::Error::GetErrorStr(err));
		}

		bool Window::IsClosed()
		{
			GLFWwindow *win = (GLFWwindow*)m_Handle;
			return glfwWindowShouldClose(win);
		}

		void Window::Clear()
		{
			Graphics::Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void Window::Update()
		{
			GLFWwindow *win = (GLFWwindow*)m_Handle;
			glfwPollEvents();
#ifndef MC_PLATFORM_WEB         
			glfwSwapBuffers(win);
#endif
		}

		void Window::SetIcon(const Graphics::Image &img)
		{
#ifndef MC_PLATFORM_WEB
			GLFWwindow *win = (GLFWwindow*)m_Handle;
			GLFWimage image[1];
			image[0].width = img.x;
			image[0].height = img.y;
			image[0].pixels = img.pixels;

			glfwSetWindowIcon(win, 1, image);
#endif /* MC_PLATFORM_WEB */
		}

		bool Window::SetFullScreen(bool fullscreen)
		{
#ifndef MC_PLATFORM_WEB
			GLFWwindow *win = (GLFWwindow*)m_Handle;
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			if (!monitor)
				return false;

			const GLFWvidmode *mode = glfwGetVideoMode(monitor);
			if (!mode)
				return false;

			if (fullscreen)
				glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, 0);
			else
				glfwSetWindowMonitor(win, nullptr, 100, 100, m_Properties.width, m_Properties.height, 0);
#endif /* MC_PLATFORM_WEB */
			return true;
		}

		WindowProperties &Window::GetProperties()
		{
			return m_Properties;
		}

		void *Window::GetHandle()
		{
			return m_Handle;
		}
	}
}