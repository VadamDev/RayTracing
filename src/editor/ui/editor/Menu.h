#pragma once

namespace editor
{
    namespace menu
    {
        static constexpr auto RENDER_SETTINGS_MENU = "Render Settings";
    }

    struct MenuEvent
    {
        std::string_view name;

        explicit MenuEvent(const std::string_view name)
            : name(name) {}
    };

    struct OpenMenuEvent : MenuEvent
    {
        using MenuEvent::MenuEvent;
    };

    struct CloseMenuEvent : MenuEvent
    {
        using MenuEvent::MenuEvent;
    };
}
