#include "InspectorPanel.h"

#include <spdlog/spdlog.h>

#include "../../scene/Components.h"

namespace application
{
    void InspectorPanel::draw()
    {
        ImGui::Begin(name.c_str());

        if (const engine::Entity &selectedEntity = hierarchyPanel->selectedEntity)
            drawComponents(selectedEntity);

        ImGui::End();
    }

    /*
     * Inspect components
     */

    void InspectorPanel::drawComponents(const engine::Entity &entity)
    {
        //Display Tag
        if (entity.hasComponent<engine::TagComponent>())
        {
            auto &tag = entity.getComponent<engine::TagComponent>().tag;

            char buffer[256] = {};
            strcpy_s(buffer, tag.c_str());

            ImGui::SetNextItemWidth(196);
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
                tag = std::string(buffer);

            //Add components button
            ImGui::SameLine();
            drawAddComponentsPopup(entity);

            ImGui::NewLine();
        }


        //Transform
        static bool linkedTransformScale = true;
        drawComponent<TransformComponent>(entity, "Transform", [](TransformComponent &transform) {
            Drag3f("Position", transform.position, 0.01f, 0, 0, "%.2f");
            Drag3f("Rotation", transform.rotation, 0.1f, 0, 0, "%.1f");
            DragLinked3f("Scale", transform.scale, linkedTransformScale, 0.01f, 0, std::numeric_limits<float>::infinity(), "%.2f");
        });

        //Raytraced Material
        drawComponent<RaytracedMaterialComponent>(entity, "Raytraced Material", [](RaytracedMaterialComponent &material) {
            Color3f("Color", material.color);
            Color3f("Emission Color", material.emissionColor);
            Drag1f("Emission Strength", material.emissionStrength, 0.01f, 0, std::numeric_limits<float>::infinity(), "%.2f");
            Drag1f("Smoothness", material.smoothness, 0.01f, 0, 1, "%.2f");
        });

        //Raytraced Sphere
        drawEmptyComponent<RaytracedSphereComponent>(entity, "Raytraced Sphere");

        //Raytraced Box
        drawEmptyComponent<RaytracedBoxComponent>(entity, "Raytraced Box");

        //Raytraced Mesh
        drawComponent<RaytracedMeshComponent>(entity, "Raytraced Mesh", [](RaytracedMeshComponent &mesh) {
            InputText("Mesh Name", mesh.name);
        });
    }

    static constexpr ImGuiTreeNodeFlags DEFAULT_COMPONENTS_FLAGS = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

    template<typename T>
    void InspectorPanel::drawComponent(const engine::Entity &entity, const std::string &name, std::function<void(T&)> drawFunc, const bool removable)
    {
        if(!entity.hasComponent<T>())
            return;

        const bool opened = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), DEFAULT_COMPONENTS_FLAGS, name.c_str());

        if (removable)
        {
            ImGui::PushID(typeid(T).name());

            ImGui::SameLine(ImGui::GetWindowWidth() - 62);
            if (ImGui::Button("Remove"))
                entity.removeComponent<T>();

            ImGui::PopID();
        }

        if (opened)
        {
            drawFunc(entity.getComponent<T>());
            ImGui::TreePop();
        }

        ImGui::NewLine();
    }

    static constexpr ImGuiTreeNodeFlags DEFAULT_EMPTY_COMPONENTS_FLAGS = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

    template<typename T>
    void InspectorPanel::drawEmptyComponent(const engine::Entity &entity, const std::string &name, const bool removable)
    {
        if (!entity.hasComponent<T>())
            return;

        ImGui::BeginDisabled();
        ImGui::TreeNodeEx((void*) typeid(T).hash_code(), DEFAULT_EMPTY_COMPONENTS_FLAGS, name.c_str());
        ImGui::EndDisabled();

        if (removable)
        {
            ImGui::PushID(typeid(T).name());

            ImGui::SameLine(ImGui::GetWindowWidth() - 62);
            if (ImGui::Button("Remove"))
                entity.removeComponent<T>();

            ImGui::PopID();
        }

        ImGui::NewLine();
    }

    /*
     * Add Components
     */

    void InspectorPanel::drawAddComponentsPopup(const engine::Entity &entity)
    {
        if(ImGui::Button("Add Components"))
            ImGui::OpenPopup("AddComponents");

        if (!ImGui::BeginPopup("AddComponents"))
            return;

        //Transform
        drawAddComponent<TransformComponent>(entity, "Transform");

        //Raytraced Material
        drawAddComponent<RaytracedMaterialComponent>(entity, "Raytraced Material");

        //Raytraced Sphere
        drawAddComponent<RaytracedSphereComponent>(entity, "Raytraced Sphere");

        //Raytraced Box
        drawAddComponent<RaytracedBoxComponent>(entity, "Raytraced Box");

        //Raytraced Mesh
        drawAddComponent<RaytracedMeshComponent>(entity, "Raytraced Mesh");

        ImGui::EndPopup();
    }

    template<typename T, typename... Args>
    void InspectorPanel::drawAddComponent(const engine::Entity &entity, const std::string &name, Args&&... args)
    {
        const bool isDisabled = entity.hasComponent<T>();
        if (isDisabled)
            ImGui::BeginDisabled();

        if (ImGui::MenuItem(name.c_str()) && !isDisabled)
        {
            entity.addComponent<T>(std::forward<Args>(args)...);
            ImGui::CloseCurrentPopup();
        }

        if (isDisabled)
            ImGui::EndDisabled();
    }
}
