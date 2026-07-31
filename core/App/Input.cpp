#include "App/Input.h"

#include "Log/Log.h"
#include "Events/EventManager.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#define CHECK_KEY(x, max) x >= 0 && x <= max

namespace MC 
{
	namespace App 
	{
		bool Input::m_Keys[MC_MAX_KEYS];
		bool Input::m_Buttons[MC_MAX_BUTTONS];
		double Input::m_dx = 0.0;
		double Input::m_dy = 0.0;

		u32t Input::m_KeyID;
		u32t Input::m_ButtonID;
		u32t Input::m_CursorID;

		void Input::Init()
		{
			/* Initialize key array */
			for (int i = 0; i < MC_MAX_KEYS; i++) {
				m_Keys[i] = 0;
			}

			/* Initialize button array */
			for (int i = 0; i < MC_MAX_BUTTONS; i++) {
				m_Buttons[i] = 0;
			}

			m_KeyID = REGISTER_EVENT(Events::EventType::KeyBoardButton, Input::ProcessEvent);
			m_ButtonID = REGISTER_EVENT(Events::EventType::MouseButton, Input::ProcessEvent);
			m_CursorID = REGISTER_EVENT(Events::EventType::CursorMoved, Input::ProcessEvent);
		}

		void Input::Finish()
		{
			UNREGISTER_EVENT(Events::EventType::KeyBoardButton, m_KeyID);
			UNREGISTER_EVENT(Events::EventType::MouseButton, m_ButtonID);
			UNREGISTER_EVENT(Events::EventType::CursorMoved, m_CursorID);
			mc_info("input system finished");
		}

		void Input::ProcessEvent(Events::Event &ev)
		{
			Events::EventType type = static_cast<Events::EventType>(ev.type);
			switch (type) {
			case Events::EventType::KeyBoardButton:
			{
				auto& e = static_cast<Events::KeyboardButtonEvent&>(ev);
				if (CHECK_KEY(e.key, MC_MAX_KEYS))
					m_Keys[e.key] = (e.action != Events::ButtonAction::Release);

				break;
			}
			case Events::EventType::MouseButton:
			{
				auto& e = static_cast<Events::MouseButtonEvent&>(ev);
				if (CHECK_KEY(e.button, MC_MAX_KEYS))
					m_Buttons[e.button] = (e.action != Events::ButtonAction::Release);

				break;
			}
				
			case Events::EventType::CursorMoved:
			{
				auto& e = static_cast<Events::CursorMotionEvent&>(ev);
				m_dx = e.dx;
				m_dy = e.dy;
				break;
			}
			default:
				break;
			}
		}

		bool Input::IsKeyPressed(int key)
		{
			return CHECK_KEY(key, MC_MAX_KEYS) ? m_Keys[key] : false;
		}

		bool Input::IsMouseButtonPressed(int button)
		{
			return CHECK_KEY(button, MC_MAX_BUTTONS) ? m_Buttons[button] : false;
		}

		double Input::GetMouseX()
		{
			return m_dx;
		}

		double Input::GetMouseY()
		{
			return m_dy;
		}

	}
}
