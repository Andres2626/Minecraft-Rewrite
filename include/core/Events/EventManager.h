#pragma once

#include "Event.h"

#include "common.h"

#include <queue>
#include <functional>

#define QUEUE_EVENT(evclass, ...) MC::Events::EventManager::Get().QueueEvent(std::make_unique<evclass>(__VA_ARGS__));
#define REGISTER_EVENT(type, func) MC::Events::EventManager::Get().Register((u32t)type, func)
#define UNREGISTER_EVENT(type, id) MC::Events::EventManager::Get().Unregister((u32t)type, id)
#define DISPATCH_EVENTS() MC::Events::EventManager::Get().DispatchEvents()

namespace MC 
{
	namespace Events 
	{        
        using EventCallback = std::function<void(Event&)>;

        struct EventListener
        {
            u32t id;
            EventCallback callback;
        };

		class MC_API EventManager
        {
        public:
           
        private:
            static u32t m_NextListenerID;
        public:
            EventManager() = default;
            ~EventManager() = default;

            EventManager(const EventManager&) = delete;
            EventManager& operator=(const EventManager&) = delete;
        private:
            std::queue<std::unique_ptr<Event>> m_EventQueue;
            std::unordered_map<u32t, std::vector<EventListener>> m_Callbacks;
        public:
            u32t Register(u32t type, EventCallback callback);
            void Unregister(u32t type, u32t id);
            void QueueEvent(std::unique_ptr<Event> ev);
            void DispatchEvents();
        public:
            static EventManager &Get();
        };
	}
}
