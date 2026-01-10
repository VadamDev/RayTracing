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

        glm::dvec2 getMousePos() const { return mouse.getPos(); }
        glm::dvec2 getMouseDelta() const { return mouse.getDelta(); }

        float getMouseScrollX() const { return mouse.scrollX; }
        float getMouseScrollY() const { return mouse.scrollY; }

        /*
         * Keyboard
         */

        bool isKeyDown(const int key) { return keyboard.isKeyDown(key); }

        Mouse& getMouse() { return mouse; }
        Keyboard& getKeyboard() { return keyboard; }

    private:
        Mouse mouse;
        Keyboard keyboard;
    };
}
