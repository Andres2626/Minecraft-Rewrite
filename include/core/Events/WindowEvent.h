#pragma once

#include "Event.h"

namespace MC 
{
	namespace Events 
	{

		struct WindowMovedEvent : public Event
		{
			int x;
			int y;

			WindowMovedEvent(int posx, int posy)
				: Event((u32t)EventType::WindowMoved), x(posx), y(posy)
			{ }
		};

		struct WindowResizeEvent : public Event
		{
			int width;
			int height;

			WindowResizeEvent(int w, int h)
				: Event((u32t)EventType::WindowResized), width(w), height(h)
			{ }
		};

		struct WindowCloseEvent : public Event
		{
			WindowCloseEvent()
				: Event((u32t)EventType::WindowClosed)
			{ }
		};

		struct WindowRefreshEvent : public Event
		{
			WindowRefreshEvent()
				: Event((u32t)EventType::WindowRefresh)
			{ }
		};

		struct WindowFocusEvent : public Event
		{
			int focus;

			WindowFocusEvent(int f)
				: Event((u32t)EventType::WindowFocused), focus(f)
			{ }
		};

		struct WindowIconifyEvent : public Event
		{
			int iconify;

			WindowIconifyEvent(int i)
				: Event((u32t)EventType::WindowIconified), iconify(i)
			{ }
		};

		struct WindowMaximizeEvent : public Event
		{
			int maximized;

			WindowMaximizeEvent(int max)
				: Event((u32t)EventType::WindowMaximized), maximized(max)
			{ }
		};

		struct WindowScaledEvent : public Event
		{
			float x;
			float y;

			WindowScaledEvent(float sx, float sy)
				: Event((u32t)EventType::WindowScaleChanged), x(sx), y(sy)
			{ }
		};
	}
}
