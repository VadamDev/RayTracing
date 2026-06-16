#pragma once

#include <string>

namespace editor
{
    class UIPanel
    {

    public:
        explicit UIPanel(std::string name)
            : name(std::move(name)) {}
        virtual ~UIPanel() = default;

        virtual void draw(float deltaTime) = 0;
        const char* getName() const { return name.c_str(); }

    protected:
        std::string name;
    };
}
