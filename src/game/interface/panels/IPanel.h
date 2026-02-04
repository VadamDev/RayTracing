#pragma once

#include <string>
#include <utility>

namespace application
{
    class IPanel
    {

    public:
        explicit IPanel(std::string name)
            : name(std::move(name)) {}
        virtual ~IPanel() = default;

        virtual void draw() = 0;

        const char* getName() const { return name.c_str(); }

    protected:
        std::string name;
    };
}