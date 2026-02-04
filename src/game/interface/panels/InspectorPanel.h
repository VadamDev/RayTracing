#pragma once

#include "HierarchyPanel.h"
#include "IPanel.h"

namespace application
{
    class InspectorPanel : public IPanel
    {

    public:
        explicit InspectorPanel(HierarchyPanel *hierarchyPanel)
            : IPanel("Inspector"), hierarchyPanel(hierarchyPanel) {}

        void draw() override;

    private:
        HierarchyPanel *hierarchyPanel;

        /*
         * Inspect components
         */

        static void drawComponents(const engine::Entity &entity);

        template<typename T>
        static void drawComponent(const engine::Entity &entity, const std::string &name, std::function<void(T&)> drawFunc, bool removable = true);

        /*
         * Add Components
         */

        static void drawAddComponentsPopup(const engine::Entity &entity);

        template<typename T, typename... Args>
        static void drawAddComponent(const engine::Entity &entity, const std::string &name, Args&&... args);
    };
}
