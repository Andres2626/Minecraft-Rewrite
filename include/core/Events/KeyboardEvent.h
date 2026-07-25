#pragma once

#include "Event.h"

namespace MC 
{
	namespace Events 
	{
		struct KeyboardButtonEvent : public Event
		{
			int key;
			int scancode;
			ButtonAction action;
			int mods;

			KeyboardButtonEvent(int k, int s, ButtonAction a, int m)
				: Event((u32t)EventType::KeyBoardButton), key(k), scancode(s), action(a), mods(m)
			{ }
		};
	}
}
