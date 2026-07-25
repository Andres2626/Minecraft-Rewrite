#pragma once

#include "Event.h"

namespace MC 
{
	namespace Events 
	{
		struct MonitorEvent : public Event
		{
			int event;

			MonitorEvent(int ev)
				: Event((u32t)EventType::Monitor), event(ev)
			{ }
		};
	}
}
