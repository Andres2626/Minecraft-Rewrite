#pragma once
#include "common.h"

#include "InputDefs.h"

#include "Events/Event.h"

#define MC_MAX_KEYS 349
#define MC_MAX_BUTTONS 9

namespace MC 
{
	namespace App 
	{
		class MC_API Input 
		{
		private:
			static bool m_Keys[MC_MAX_KEYS];
			static bool m_Buttons[MC_MAX_BUTTONS];
			static double m_X;
			static double m_Y;

			/* event destroy handling TODO: improve this! */
			static u32t m_KeyID;
			static u32t m_ButtonID;
			static u32t m_CursorID;
		public:
			static void Init();
			static void Finish();
			static void ProcessEvent(Events::Event &ev);
			static bool IsKeyPressed(int key);
			static bool IsMouseButtonPressed(int button);
		public:
			static double GetMouseX();
			static double GetMouseY();
		};

	}
}