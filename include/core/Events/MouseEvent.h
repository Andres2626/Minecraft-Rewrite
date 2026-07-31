#pragma once

#include "Event.h"

namespace MC 
{
	namespace Events 
	{
		struct MouseButtonEvent : public Event
		{
			int button;
			ButtonAction action;
			int mods;

			MouseButtonEvent(int b, ButtonAction a, int m)
				: Event((u32t)EventType::MouseButton), button(b), action(a), mods(m)
			{ }
		};

		struct CursorMotionEvent : public Event
		{
			double dx;
			double dy;

			CursorMotionEvent(double x, double y)
				: Event((u32t)EventType::CursorMoved), dx(x), dy(y)
			{ }
		};

		struct CursorEnterEvent : public Event
		{
			int entered;

			CursorEnterEvent(int e)
				: Event((u32t)EventType::CursorEntered), entered(e)
			{ }
		};

		struct MouseScrollEvent : public Event
		{
			double xoffset;
			double yoffset;

			MouseScrollEvent(double x, double y)
				: Event((u32t)EventType::MouseScrolled), xoffset(x), yoffset(y)
			{ }
		};
	}
}
