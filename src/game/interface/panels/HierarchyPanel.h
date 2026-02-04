#pragma once

#include "IPanel.h"
#include "../../../engine/scene/Entity.h"

namespace application
{
    class RaytracingApplication;

    class HierarchyPanel : public IPanel
    {

    public:
        explicit HierarchyPanel(RaytracingApplication *application)
            : IPanel("Hierarchy"), application(application) {}

        void draw() override;

        engine::Entity selectedEntity;
    private:
        RaytracingApplication *application;

        void drawHierarchyPopup(engine::Scene &scene);

        void drawEntity(engine::Scene &scene, const entt::entity &handle, const engine::Entity &entity);
        void drawEntityPopup(engine::Scene &scene, const engine::Entity &entity);
    };
}
