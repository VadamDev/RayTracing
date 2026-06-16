#pragma once

#include <functional>

#include "../UIPanel.h"

namespace engine
{
    class Entity;
}

namespace editor
{
    class SceneHandler;
    class HierarchyPanel;

    class InspectorPanel : public UIPanel
    {

    public:
        explicit InspectorPanel(SceneHandler *sceneHandler, HierarchyPanel *hierarchyPanel)
            : UIPanel("Inspector"), sceneHandler(sceneHandler), hierarchyPanel(hierarchyPanel) {}

        void draw(float deltaTime) override;

    private:
        SceneHandler *sceneHandler;
        HierarchyPanel *hierarchyPanel;

        void drawComponents(const engine::Entity &entity);

        void drawAddComponentsPopup(const engine::Entity &entity);
        template<typename T, typename... Args>
        void drawAddComponent(const engine::Entity &entity, const std::string &name, Args&&... args);

        template<typename T>
        static void drawComponent(const engine::Entity &entity, const std::string &name, std::function<void(T&)> drawFunc, bool removable = true);

        template<typename T>
        static void drawEmptyComponent(const engine::Entity &entity, const std::string &name, bool removable = true);
    };
}
