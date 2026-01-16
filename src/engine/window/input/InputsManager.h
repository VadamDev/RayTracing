#pragma once

#include "Mouse.h"
#include "Keyboard.h"

namespace engine
{
    class InputsManager
    {

    public:
        /*
         * Mouse
         */

        bool isMouseButtonDown(const int button) const { return mouse.isPressed(button); }
        bool isMouseButtonDown(const MouseButton button) const { return isMouseButtonDown(static_cast<int>(button)); }

        glm::dvec2& getMousePos() { return mouse.getPos(); }
        glm::dvec2& getMouseDelta() { return mouse.getDelta(); }

        float getMouseScrollX() const { return mouse.scrollX; }
        float getMouseScrollY() const { return mouse.scrollY; }

        /*
         * Keyboard
         */

        bool isKeyDown(const int key) const { return keyboard.isKeyDown(key); }
        bool isKeyDown(const KeyboardKeys key) const { return isKeyDown(static_cast<int>(key)); }

        Mouse& getMouse() { return mouse; }
        Keyboard& getKeyboard() { return keyboard; }

    private:
        Mouse mouse;
        Keyboard keyboard;
    };
}
