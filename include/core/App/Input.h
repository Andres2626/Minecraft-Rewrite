#pragma once
#include "common.h"

#include "InputDefs.h"

#include "Events/EventManager.h"
#include "Math/Math.h"

#define MC_MAX_KEYS 349
#define MC_MAX_BUTTONS 9

namespace MC 
{
	namespace App 
	{
		class MC_API Input : public Events::IEventListener
		{
		public:
			/**
			  * \brief Initialize mouse, cursor and keyboard events for 
			  * handle window input.
			  */
			static void Init();

			/**
			  * \brief Finish input events.
			  */
			static void Finish();

			/**
			  * \brief Check if key is pressed.
			  * 
			  * \return true on key pressed, false otherwise.
			  */
			static bool IsKeyPressed(int key);

			/**
			  * \brief Check if mouse button is pressed.
			  *
			  * \return true on button pressed, false otherwise.
			  */
			static bool IsMouseButtonPressed(int button);

			/**
			  * \brief Change cursor visibility on window.
			  * 
			  * \param handle Window handle.
			  * \param state Cursor flag.
			  */
			static void SetCursorState(void *handle, int state);
        public:
            static void SetMouseFirst(bool first);
            static void SetMouseLast(const Math::vec2 &last);
		public:
			static double GetMouseX();
			static double GetMouseY();
            static bool GetMouseFirst();
			static Math::vec2 GetMouseLast();
		private:
			void OnEvent(Events::Event& ev) override;
		};

	}
}