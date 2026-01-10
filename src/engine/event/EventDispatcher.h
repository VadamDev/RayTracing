#pragma once

#include <functional>
#include <vector>
#include <spdlog/spdlog.h>

#include "IEvent.h"

namespace engine
{
    template <std::derived_from<IEvent> T>
    class EventDispatcher
    {

    public:
        using Listener = std::function<void(T&)>;

        ~EventDispatcher()
        {
            unregisterAll();
        }

        void subscribe(Listener listener)
        {
            listeners.push_back(listener);
        }

        void dispatch(T &event)
        {
            for (auto func : listeners)
            {
                func(event);

                if (!event.canPropagate())
                    break;
            }
        }

        void unregisterAll()
        {
            listeners.clear();
        }

    private:
        std::vector<Listener> listeners;
    };
}
