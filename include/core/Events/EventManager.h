#pragma once

#include "common.h"

#include "Event.h"

#include "Utils/NonCopyable.h"

#define QUEUE_EVENT(evclass, ...) MC::Events::EventManager::Get().QueueEvent(new evclass(__VA_ARGS__));
#define REGISTER_EVENT(type, func) MC::Events::EventManager::Get().Register((u32t)type, func)
#define UNREGISTER_EVENT(type, lis) MC::Events::EventManager::Get().Unregister((u32t)type, lis)
#define DISPATCH_EVENTS() MC::Events::EventManager::Get().DispatchEvents()

namespace MC 
{
	namespace Events 
	{   

        class MC_API IEventListener
        {
        public:
            IEventListener() = default;
            virtual ~IEventListener() = default;
            virtual void OnEvent(Event &ev) = 0;
        };

        struct EventListener
        {
            u32t id;
            IEventListener *listener;
        };

		class MC_API EventManager : public NonCopyable
        {
        public:
            EventManager() = default;
            ~EventManager() = default;
        public:
            u32t Register(u32t type, IEventListener *listener);
            void Unregister(u32t type, u32t id);
            void QueueEvent(Event *ev);
            void DispatchEvents();
        public:
            static EventManager &Get();
        };
	}
}
