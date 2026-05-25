#include "HierarchyPanel.h"

#include "../../scene/SceneHandler.h"
#include "../../scene/Components.h"

namespace editor
{
    void HierarchyPanel::draw()
    {
        ImGui::Begin(getName());

        if (sceneHandler->isSceneOpened())
        {
            engine::Scene *scene = sceneHandler->getOpenedScene();

            if (selectedEntity)
            {
                // Deselect entity if clicked on nothing OR escape key is pressed
                if ((ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) || ImGui::IsKeyPressed(ImGuiKey_Escape))
                {
                    selectedEntity = {};
                }
                // Quick remove entity by pressing delete
                else if (ImGui::IsKeyPressed(ImGuiKey_Delete))
                {
                    scene->destroyEntity(selectedEntity);
                    selectedEntity = {};
                }
            }

            // Draw all entity present in the scene
            for (auto &entityHandle : scene->registry.view<entt::entity>())
            {
                const engine::Entity entity(entityHandle, scene);
                drawEntity(scene, entityHandle, entity);
            }

            // Right click on panel popup (add entities)
            if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
            {
                drawHierarchyCtxPopup(scene);
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }

    void HierarchyPanel::drawHierarchyCtxPopup(engine::Scene *scene)
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
            // todo: resetAccumulation();
        }

        // Raytraced Box
        if (ImGui::MenuItem("New [Raytraced Box]"))
        {
            const engine::Entity box = scene->newEntity("Raytraced Box");

            box.addComponent<TransformComponent>();
            box.addComponent<RaytracedMaterialComponent>();
            box.addComponent<RaytracedMeshComponent>().name = "cube";

            selectedEntity = box;
            // todo: resetAccumulation();
        }

        // Raytraced Mesh
        if (ImGui::MenuItem("New [Raytraced Mesh]"))
        {
            const engine::Entity mesh = scene->newEntity("Raytraced Mesh");

            mesh.addComponent<TransformComponent>();
            mesh.addComponent<RaytracedMaterialComponent>();
            mesh.addComponent<RaytracedMeshComponent>();

            selectedEntity = mesh;
            // todo: resetAccumulation();
        }
    }

    void HierarchyPanel::drawEntity(engine::Scene *scene, const entt::entity &handle, const engine::Entity &entity)
    {
        auto tag = std::string("Unknown");
        if (entity.hasComponent<engine::TagComponent>())
            tag = entity.getComponent<engine::TagComponent>().tag;

        const ImGuiTreeNodeFlags FLAGS = (selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Bullet;
        const bool opened = ImGui::TreeNodeEx((void*) (uint64_t) (uint32_t) handle, FLAGS, tag.c_str());

        // Select the entity on left click
        if (ImGui::IsItemClicked())
            selectedEntity = entity;

        // Right click context menu (Duplicate / Delete entity)
        if (ImGui::BeginPopupContextItem())
        {
            drawEntityCtxPopup(scene, entity);
            ImGui::EndPopup();
        }

        if (opened)
            ImGui::TreePop();
    }

    void HierarchyPanel::drawEntityCtxPopup(engine::Scene *scene, const engine::Entity &entity)
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
}
