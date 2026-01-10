#pragma once

namespace engine
{
    class IEvent
    {

    public:
        virtual ~IEvent() = default;

        void stopPropagation() { bPropagate = false; }
        bool canPropagate() const { return bPropagate; }

    private:
        bool bPropagate = true;
    };
}
