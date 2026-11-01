#include "mc.h"

#include "Events/EventManager.h"

namespace MC 
{
	namespace Events 
	{
        static u32t s_NextListenerID;
        static std::queue<std::unique_ptr<Event>> s_EventQueue;
        static std::unordered_map<u32t, std::vector<EventListener>> s_Callbacks;

		void EventManager::Unregister(u32t type, u32t id)
        {
            auto it = s_Callbacks.find(type);
            if (it == s_Callbacks.end())
                return;

            auto& listeners = it->second;
            for (auto iter = listeners.begin(); iter != listeners.end(); ++iter) {
                if (iter->id == id) {
                    listeners.erase(iter);
                    return;
                }
            }
        }

        u32t EventManager::Register(u32t type, IEventListener *listener)
        {
            EventListener el;
            el.id = s_NextListenerID++;
            el.listener = listener;
            s_Callbacks[type].push_back(el);
            return el.id;
        }

        void EventManager::QueueEvent(Event *ev)
        {
            std::unique_ptr<Event> e(ev);
            s_EventQueue.emplace(std::move(e));
        }

        void EventManager::DispatchEvents()
        {
            while (!s_EventQueue.empty())
            {
                std::unique_ptr<Event> ev = std::move(s_EventQueue.front());
                s_EventQueue.pop();

                auto it = s_Callbacks.find(ev->type);
                if (it == s_Callbacks.end())
                    continue;

                auto &v = it->second;
                for (auto &entry : v) {
                    if (entry.listener)
                        entry.listener->OnEvent(*ev);

                    if (ev->handled)
                        break;
                }
            }
        }

        EventManager& EventManager::Get()
        {
            static EventManager instance;
            return instance;
        }
	}
}
