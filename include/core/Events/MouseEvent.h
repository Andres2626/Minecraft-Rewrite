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

		struct CursorPositionEvent : public Event
		{
			double x;
			double y;

			CursorPositionEvent(double posx, double posxy)
				: Event((u32t)EventType::CursorMoved), x(posx), y(posxy)
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
