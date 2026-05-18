#include "Mouse.h"

namespace engine
{
    /*
     * Callbacks
     */

    void Mouse::onCursorPosition(const double xPos, const double yPos)
    {
        pos.x = xPos;
        pos.y = yPos;
    }

    void Mouse::onScroll(const double xOffset, const double yOffset)
    {
        scrollX += xOffset;
        scrollY += yOffset;
    }

    void Mouse::onMouseButton(const int button, const int action, const int mods)
    {
        if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST)
            return;

        //TODO: in not sure if mods is really useful here, as their will be detected by the keyboard class anyway
        buttons[button] = action != GLFW_RELEASE;
    }

    void Mouse::processDeltas()
    {
        mouseDelta.x = pos.x - oldPos.x;
        mouseDelta.y = pos.y - oldPos.y;

        oldPos.x = pos.x;
        oldPos.y = pos.y;
    }

}
