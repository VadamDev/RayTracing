#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace engine
{
    enum class MouseButton
    {
        BUTTON_1 = GLFW_MOUSE_BUTTON_1,
        BUTTON_2 = GLFW_MOUSE_BUTTON_2,
        BUTTON_3 = GLFW_MOUSE_BUTTON_3,
        BUTTON_4 = GLFW_MOUSE_BUTTON_4,
        BUTTON_5 = GLFW_MOUSE_BUTTON_5,
        BUTTON_6 = GLFW_MOUSE_BUTTON_6,
        BUTTON_7 = GLFW_MOUSE_BUTTON_7,
        BUTTON_8 = GLFW_MOUSE_BUTTON_8
    };

    class Mouse
    {

    public:
        /*
         * Callbacks
         */

        void onCursorPosition(double xPos, double yPos);
        void onScroll(double xOffset, double yOffset);
        void onMouseButton(int button, int action, int mods);

        void processDeltas();

        /*
         * Key States
         */

        bool isPressed(const int button) const { return buttons[button]; }

        /*
         * Getters
         */

        glm::dvec2& getPos() { return pos; }
        glm::dvec2& getDelta() { return mouseDelta; }

        float scrollX = 0, scrollY = 0;
    private:
        glm::dvec2 pos = glm::dvec2(0, 0), oldPos = glm::dvec2(0, 0);
        glm::dvec2 mouseDelta = glm::dvec2(0, 0);

        bool buttons[GLFW_MOUSE_BUTTON_LAST] = {};
    };
}
