#include "Events/EventManager.h"

namespace MC 
{
	namespace Events 
	{
        u32t EventManager::m_NextListenerID;

		void EventManager::Unregister(u32t type, u32t id)
        {
            auto it = m_Callbacks.find(type);

            if (it == m_Callbacks.end())
                return;

            auto& listeners = it->second;

            for (auto iter = listeners.begin(); iter != listeners.end(); ++iter)
            {
                if (iter->id == id)
                {
                    listeners.erase(iter);
                    return;
                }
            }
        }

        u32t EventManager::Register(u32t type, EventCallback callback)
        {
            EventListener listener;
            listener.id = m_NextListenerID++;
            listener.callback = std::move(callback);

            m_Callbacks[type].push_back(std::move(listener));

            return listener.id;
        }

        void EventManager::QueueEvent(std::unique_ptr<Event> ev)
        {
            m_EventQueue.emplace(std::move(ev));
        }

        void EventManager::DispatchEvents()
        {
            while (!m_EventQueue.empty())
            {
                std::unique_ptr<Event> ev = std::move(m_EventQueue.front());
                m_EventQueue.pop();

                auto it = m_Callbacks.find(ev->type);

                if (it == m_Callbacks.end())
                    continue;

                auto& v = it->second;
                for (auto& callback : v)
                {
                    callback.callback(*ev);

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
