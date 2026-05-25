#pragma once

#include "../UIPanel.h"
#include "../../../engine/scene/Entity.h"

namespace editor
{
    class SceneHandler;

    class HierarchyPanel : public UIPanel
    {

    public:
        explicit HierarchyPanel(SceneHandler *sceneHandler)
            : UIPanel("Hierarchy"), sceneHandler(sceneHandler) {}

        void draw() override;

        engine::Entity selectedEntity;
    private:
        SceneHandler *sceneHandler;

        void drawHierarchyCtxPopup(engine::Scene *scene);

        void drawEntity(engine::Scene *scene, const entt::entity &handle, const engine::Entity &entity);
        void drawEntityCtxPopup(engine::Scene *scene, const engine::Entity &entity);
    };
}
