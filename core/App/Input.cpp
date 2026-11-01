#include "mc.h"

#include "App/Input.h"

#include "Log/Log.h"
#include "Events/EventManager.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include <glfw/glfw3.h>

#define CHECK_KEY(x, max) x >= 0 && x <= max

namespace MC 
{
	namespace App 
	{
		static Input s_Instance;
		static bool s_Keys[MC_MAX_KEYS];
        static bool s_Buttons[MC_MAX_BUTTONS];
        static double s_dx;
        static double s_dy;     
        static bool s_MouseFirst;
        static Math::vec2 s_MouseLast;

        /* event destroy handling TODO: improve this! */
        static u32t s_KeyID;
        static u32t s_ButtonID;
        static u32t s_CursorID;

		void Input::Init()
		{
            s_MouseFirst = true;
            s_MouseLast = {0.0f, 0.0f};
            
			/* Initialize key array */
			for (int i = 0; i < MC_MAX_KEYS; i++) {
				s_Keys[i] = 0;
			}

			/* Initialize button array */
			for (int i = 0; i < MC_MAX_BUTTONS; i++) {
				s_Buttons[i] = 0;
			}

			s_KeyID = REGISTER_EVENT(Events::EventType::KeyBoardButton, &s_Instance);
			s_ButtonID = REGISTER_EVENT(Events::EventType::MouseButton, &s_Instance);
			s_CursorID = REGISTER_EVENT(Events::EventType::CursorMoved, &s_Instance);
		}

		void Input::Finish()
		{
			UNREGISTER_EVENT(Events::EventType::KeyBoardButton, s_KeyID);
			UNREGISTER_EVENT(Events::EventType::MouseButton, s_ButtonID);
			UNREGISTER_EVENT(Events::EventType::CursorMoved, s_CursorID);
			mc_info("input system finished");
		}

		void Input::OnEvent(Events::Event &ev)
		{
			Events::EventType type = static_cast<Events::EventType>(ev.type);
			switch (type) {
			case Events::EventType::KeyBoardButton:
			{
				auto& e = static_cast<Events::KeyboardButtonEvent&>(ev);
				if (CHECK_KEY(e.key, MC_MAX_KEYS))
					s_Keys[e.key] = (e.action != Events::ButtonAction::Release);

				break;
			}
			case Events::EventType::MouseButton:
			{
				auto& e = static_cast<Events::MouseButtonEvent&>(ev);
				if (CHECK_KEY(e.button, MC_MAX_KEYS))
					s_Buttons[e.button] = (e.action != Events::ButtonAction::Release);

				break;
			}
				
			case Events::EventType::CursorMoved:
			{
				auto& e = static_cast<Events::CursorMotionEvent&>(ev);
				s_dx = e.dx;
				s_dy = e.dy;
				break;
			}
			default:
				break;
			}
		}

		bool Input::IsKeyPressed(int key)
		{
			return CHECK_KEY(key, MC_MAX_KEYS) ? s_Keys[key] : false;
		}

		bool Input::IsMouseButtonPressed(int button)
		{
			return CHECK_KEY(button, MC_MAX_BUTTONS) ? s_Buttons[button] : false;
		}

		void Input::SetCursorState(void *handle, int state)
		{
			glfwSetInputMode((GLFWwindow*)handle, GLFW_CURSOR, state);
		}

		double Input::GetMouseX()
		{
			return s_dx;
		}

		double Input::GetMouseY()
		{
			return s_dy;
		}
        
        void Input::SetMouseFirst(bool first)
        {
            s_MouseFirst = first;
        }
        
        void Input::SetMouseLast(const Math::vec2& last)
        {
            s_MouseLast = last;
        }
        
        bool Input::GetMouseFirst()
		{
			return s_MouseFirst;
		}

		Math::vec2 Input::GetMouseLast()
		{
			return s_MouseLast;
		}
	}
}
