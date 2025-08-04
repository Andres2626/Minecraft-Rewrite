#include "Input.h"

#define CHECK_KEY(x, max) x >= 0 && x <= max

namespace MC {
	namespace App {

		bool Input::m_Keys[MC_MAX_KEYS];
		bool Input::m_Buttons[MC_MAX_BUTTONS];
		double Input::m_X = 0.0;
		double Input::m_Y = 0.0;

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
		}

		void Input::ProcessEvent(const Events::Event& ev)
		{
			switch (ev.type) {
			case GLEQ_KEY_PRESSED:
				if (CHECK_KEY(ev.keyboard.key, MC_MAX_KEYS))
					m_Keys[ev.keyboard.key] = true;
				break;
			case GLEQ_KEY_RELEASED:
				if (CHECK_KEY(ev.keyboard.key, MC_MAX_KEYS))
					m_Keys[ev.keyboard.key] = false;
				break;
			case GLEQ_BUTTON_PRESSED:
				if (CHECK_KEY(ev.mouse.button, MC_MAX_BUTTONS))
					m_Buttons[ev.mouse.button] = true;
				break;
			case GLEQ_BUTTON_RELEASED:
				if (CHECK_KEY(ev.mouse.button, MC_MAX_BUTTONS))
					m_Buttons[ev.mouse.button] = false;
				break;
			case GLEQ_CURSOR_MOVED:
				m_X = ev.pos.x;
				m_Y = ev.pos.y;
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
			return m_X;
		}

		double Input::GetMouseY()
		{
			return m_Y;
		}

	}
}
