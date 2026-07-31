#pragma once

#include "../../UIPanel.h"
#include "../../../../engine/scene/Entity.h"

namespace engine
{
    class Window;
}

namespace editor
{
    class SceneHandler;

    class HierarchyPanel : public UIPanel
    {

    public:
        explicit HierarchyPanel(engine::Window &window, SceneHandler *sceneHandler)
            : UIPanel("Hierarchy"), window(window), sceneHandler(sceneHandler) {}

        void draw(float deltaTime) override;

        engine::Entity selectedEntity;
    private:
        engine::Window &window;
        SceneHandler *sceneHandler;

        void drawHierarchyCtxPopup(engine::Scene *scene);

        void drawEntity(engine::Scene *scene, const entt::entity &handle, const engine::Entity &entity);
        void drawEntityCtxPopup(engine::Scene *scene, const engine::Entity &entity);

        void resetAccumulation() const;
    };
}
