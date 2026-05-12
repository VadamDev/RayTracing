#include "HierarchyPanel.h"

#include "../../RaytracingApplication.h"
#include "../../scene/Components.h"

namespace application
{
    void HierarchyPanel::draw()
    {
        ImGui::Begin(name.c_str());

        engine::Scene *scene = application->getActiveScene();

        // Unselect entity if clicked on nothing
        if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
            selectedEntity = {};

        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            selectedEntity = {};

        // Quick remove entity
        const auto &inputsManager= application->getWindow().getInputsManager();
        if (selectedEntity && inputsManager->isKeyDown(engine::KeyboardKeys::KEY_DELETE) && !ImGui::GetIO().WantCaptureKeyboard)
        {
            scene->destroyEntity(selectedEntity);
            selectedEntity = {};
        }

        // Draw all scene entities in the panel
        for (auto &entityHandle : scene->registry.view<entt::entity>())
        {
            const engine::Entity entity(entityHandle, scene);
            drawEntity(scene, entityHandle, entity);
        }

        // Right click hierarchy popup
        if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
            drawHierarchyPopup(scene);
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void HierarchyPanel::drawHierarchyPopup(engine::Scene *scene)
    {
        // Empty Entity
        if (ImGui::MenuItem("New [Empty Entity]"))
            scene->newEntity("Empty Entity");

        // Raytraced Sphere
        if (ImGui::MenuItem("New [Raytraced Sphere]"))
        {
            const engine::Entity sphere = scene->newEntity("Raytraced Sphere");

            sphere.addComponent<TransformComponent>();
            sphere.addComponent<RaytracedMaterialComponent>();
            sphere.addComponent<RaytracedSphereComponent>();

            selectedEntity = sphere;
            resetAccumulation();
        }

        // Raytraced Box
        if (ImGui::MenuItem("New [Raytraced Box]"))
        {
            const engine::Entity box = scene->newEntity("Raytraced Box");

            box.addComponent<TransformComponent>();
            box.addComponent<RaytracedMaterialComponent>();
            box.addComponent<RaytracedMeshComponent>().name = "cube";

            selectedEntity = box;
            resetAccumulation();
        }

        // Raytraced Mesh
        if (ImGui::MenuItem("New [Raytraced Mesh]"))
        {
            const engine::Entity mesh = scene->newEntity("Raytraced Mesh");

            mesh.addComponent<TransformComponent>();
            mesh.addComponent<RaytracedMaterialComponent>();
            mesh.addComponent<RaytracedMeshComponent>();
            
            selectedEntity = mesh;
            resetAccumulation();
        }
    }

    void HierarchyPanel::drawEntity(engine::Scene *scene, const entt::entity &handle, const engine::Entity &entity)
    {
        // Just in case
        auto tag = std::string("UNKNOWN");
        if (entity.hasComponent<engine::TagComponent>())
            tag = entity.getComponent<engine::TagComponent>().tag;

        const ImGuiTreeNodeFlags FLAGS = (selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Bullet;
        const bool opened = ImGui::TreeNodeEx((void*) (uint64_t) (uint32_t) handle, FLAGS, tag.c_str());

        // Select the clicked entity
        if (ImGui::IsItemClicked())
            selectedEntity = entity;

        // Create right click context menu
        if (ImGui::BeginPopupContextItem())
        {
            drawEntityPopup(scene, entity);
            ImGui::EndPopup();
        }

        if (opened)
            ImGui::TreePop();
    }

    void HierarchyPanel::drawEntityPopup(engine::Scene *scene, const engine::Entity &entity)
    {
        // Copy Entity
        if (ImGui::MenuItem("Duplicate"))
        {
            std::string tag = "UNKNOWN";
            if(entity.hasComponent<engine::TagComponent>())
                tag = std::string(entity.getComponent<engine::TagComponent>().tag);

            selectedEntity = scene->copyEntity(entity, tag);

            return;
        }

        // Destroy entity
        if (ImGui::MenuItem("Delete"))
        {
            if (selectedEntity == entity)
                selectedEntity = {};

            scene->destroyEntity(entity);
        }
    }

    void HierarchyPanel::resetAccumulation() const
    {
        AccumulationResetEvent event;
        application->getGlobalMessenger()->dispatch(event);
    }
}
