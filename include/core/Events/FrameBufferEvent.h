#pragma once

#include "Event.h"

namespace MC 
{
	namespace Events 
	{
		struct FrameBufferResizeEvent : public Event
		{
			int width;
			int height;

			FrameBufferResizeEvent(int x, int y)
				: Event((u32t)EventType::FrameBufferResized), width(x), height(y)
			{ }
		};
	}
}
