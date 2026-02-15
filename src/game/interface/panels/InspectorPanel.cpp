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
        drawComponent<TransformComponent>(entity, "Transform", [](TransformComponent &transform) {
            Drag3f("Position", transform.position, 0.01f, 0, 0, "%.2f");
            Drag3f("Rotation", transform.rotation, 0.01f, 0, 0, "%.2f");
            Drag1f("Scale", transform.scale, 0.01f, 0, std::numeric_limits<float>::infinity(), "%.2f");
        });

        //Raytraced Material
        drawComponent<RaytracedMaterialComponent>(entity, "Raytraced Material", [](RaytracedMaterialComponent &material) {
            Color4f("Color", material.color);
        });

        //Raytraced Sphere
        drawComponent<RaytracedSphereComponent>(entity, "Raytraced Sphere", [](RaytracedSphereComponent &sphere) {});
    }

    static constexpr ImGuiTreeNodeFlags DEFAULT_COMPONENTS_FLAGS = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

    template<typename T>
    void InspectorPanel::drawComponent(const engine::Entity &entity, const std::string &name, std::function<void(T&)> drawFunc, const bool removable)
    {
        if(!entity.hasComponent<T>())
            return;

        auto &component = entity.getComponent<T>();
        if (ImGui::TreeNodeEx((void*) typeid(T).hash_code(), DEFAULT_COMPONENTS_FLAGS, name.c_str()))
        {
            if (removable)
            {
                ImGui::SameLine(ImGui::GetWindowWidth() - 62);

                if (ImGui::Button("Remove"))
                    entity.removeComponent<T>();
           }

            drawFunc(component);
            ImGui::TreePop();
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
