#pragma once

#include "Event.h"

namespace MC 
{
	namespace Events 
	{
		struct JoystickEvent : public Event
		{
			int id;
			int event;

			JoystickEvent(int i, int ev)
				: Event((u32t)EventType::Joystick), id(i), event(ev)
			{ }
		};
	}
}
