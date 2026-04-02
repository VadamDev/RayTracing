#pragma once

#include <functional>
#include <unordered_map>

// Inspired of Zeroconf's Messenger, see https://github.com/xeroconf/messenger
// This class is not thread safe, and offer less control
namespace engine
{
    /*
     * Subscription
     */

    class Subscription
    {
        friend class Messenger;

    public:
        explicit Subscription(const std::function<void(void*)> &callback, const bool ephemeral)
            : callback(callback), ephemeral(ephemeral) {}

    private:
        std::function<void(void*)> callback;
        bool ephemeral;
    };

    /*
     * Messenger
     */

    class Messenger
    {
        using MsgTypeId = uintptr_t;

    public:
        ~Messenger()
        {
            subscriptions.clear();
        }

        template<typename T>
        void subscribe(const std::function<void(T*)> &callback, const bool ephemeral = false)
        {
            const MsgTypeId typeId = getMessageTypeId<T>();

            const Subscription sub([callback](void *msg) { callback(static_cast<T*>(msg)); }, ephemeral);
            subscriptions[typeId].emplace_back(sub);
        }

        template<typename T>
        void dispatch(T &message)
        {
            const MsgTypeId typeId = getMessageTypeId<T>();

            const auto &entry = subscriptions.find(typeId);
            if (entry == subscriptions.end())
                return;

            auto &subs = entry->second;
            for (auto it = subs.begin(); it != subs.end();)
            {
                const Subscription &sub = *it;
                sub.callback(&message);

                if (sub.ephemeral)
                    subs.erase(it);
                else
                    ++it;
            }

            if (subs.empty())
                subscriptions.erase(typeId);
        }

    private:
        std::unordered_map<MsgTypeId, std::vector<Subscription>> subscriptions;

        template<typename T>
        static MsgTypeId getMessageTypeId() {
            static const char id = 0;
            return reinterpret_cast<MsgTypeId>(&id);
        }
    };
}
