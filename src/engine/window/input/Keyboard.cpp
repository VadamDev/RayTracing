#include "Keyboard.h"

namespace engine {
    void Keyboard::onKey(const int key, const int scancode, const int action, const int mods)
    {
        if (key < 0 || key >= GLFW_KEY_LAST)
            return;

        keys[key] = action != GLFW_RELEASE; //TODO: GLFW_REPEAT support ?
    }
}
