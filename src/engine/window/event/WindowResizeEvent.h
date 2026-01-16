#pragma once

#include "../../event/IEvent.h"

namespace engine
{
    class WindowResizeEvent : public IEvent
    {

    public:
        explicit WindowResizeEvent(const int newWidth, const int newHeight)
            : newWidth(newWidth), newHeight(newHeight), newAspectRatio((float) newWidth / newHeight) {}

        int getNewWidth() const { return newWidth; }
        int getNewHeight() const { return newHeight; }

        float getNewAspectRatio() const { return newAspectRatio; }

    private:
        int newWidth, newHeight;
        float newAspectRatio;
    };
}
