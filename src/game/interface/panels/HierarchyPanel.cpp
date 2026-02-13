#include "HierarchyPanel.h"

#include "../../RaytracingApplication.h"

namespace application
{
    void HierarchyPanel::draw()
    {
        ImGui::Begin(name.c_str());

        engine::Scene &scene = application->getActiveScene();

        //Draw all scene entities in the panel
        for (auto &entityHandle : scene.registry.view<entt::entity>())
        {
            const engine::Entity entity(entityHandle, &scene);
            drawEntity(scene, entityHandle, entity);
        }

        //Unselect entity if clicked on nothing
        if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
            selectedEntity = {};

        //Right click hierarchy popup
        if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
            drawHierarchyPopup(scene);
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void HierarchyPanel::drawHierarchyPopup(engine::Scene &scene)
    {
        if (ImGui::MenuItem("New Entity"))
            scene.newEntity("Entity");
    }

    void HierarchyPanel::drawEntity(engine::Scene &scene, const entt::entity &handle, const engine::Entity &entity)
    {
        //Just in case
        auto tag = std::string("UNKNOWN");
        if (entity.hasComponent<engine::TagComponent>())
            tag = entity.getComponent<engine::TagComponent>().tag;

        const ImGuiTreeNodeFlags FLAGS = (selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Bullet;
        const bool opened = ImGui::TreeNodeEx((void*) (uint64_t) (uint32_t) handle, FLAGS, tag.c_str());

        //Select the clicked entity
        if (ImGui::IsItemClicked())
            selectedEntity = entity;

        //Create right click context menu
        if (ImGui::BeginPopupContextItem())
        {
            drawEntityPopup(scene, entity);
            ImGui::EndPopup();
        }

        if (opened)
            ImGui::TreePop();
    }

    void HierarchyPanel::drawEntityPopup(engine::Scene &scene, const engine::Entity &entity)
    {
        //Copy Entity
        if (ImGui::MenuItem("Duplicate"))
        {
            std::string tag = "UNKNOWN";
            if(entity.hasComponent<engine::TagComponent>())
                tag = std::string(entity.getComponent<engine::TagComponent>().tag);

            selectedEntity = scene.copyEntity(entity, tag);

            return;
        }

        //Destroy entity
        if (ImGui::MenuItem("Delete"))
        {
            scene.destroyEntity(entity);

            if (selectedEntity == entity)
                selectedEntity = {};

            return;
        }
    }
}
