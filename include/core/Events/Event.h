#pragma once
#include "common.h"

namespace MC 
{
	namespace Events 
	{
		enum class EventType
		{
			Unused,

			/* window events */
			WindowMoved,
			WindowResized,
			WindowClosed,
			WindowRefresh,
			WindowFocused,
			WindowIconified,
			WindowMaximized,
			WindowScaleChanged,

			/* framebuffer */
			FrameBufferResized,

			/* mouse */
			MouseButton,
			CursorMoved,
			CursorEntered,
			MouseScrolled,

			/* keyboard */
			KeyBoardButton,

			/* monitor */
			Monitor,

			/* joystick */
			Joystick
		};

		enum class ButtonAction
		{
			Release = 0,
			Press,
			Repeat
		};

		struct Event
		{
			u32t type;
			bool handled = false;

			Event() = default;

			Event(u32t t)
				: type(t)
			{

			}

			virtual ~Event() = default;
		};

	}
}
