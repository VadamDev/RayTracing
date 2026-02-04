#include "InspectorPanel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "../../scene/Components.h"

namespace application
{
    void InspectorPanel::draw()
    {
        ImGui::Begin(name.c_str());

        if (const engine::Entity &selectedEntity = hierarchyPanel->selectedEntity)
        {
            drawComponents(selectedEntity);
            drawAddComponentsPopup(selectedEntity);
        }

        ImGui::End();
    }

    /*
     * Inspect components
     */

    void InspectorPanel::drawComponents(const engine::Entity &entity)
    {
        //Tag component
        if (entity.hasComponent<engine::TagComponent>())
        {
            auto &tag = entity.getComponent<engine::TagComponent>().tag;

            char buffer[256] = {};
            strcpy_s(buffer, tag.c_str());

            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                tag = std::string(buffer);

            ImGui::NewLine();
        }

        //Transform
        drawComponent<TransformComponent>(entity, "Transform", [](TransformComponent &transform) {
            ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.1f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(transform.rotation), 0.1f);
            ImGui::DragFloat("Scale", &transform.scale, 0.1f);
        });
    }

    template<typename T>
    void InspectorPanel::drawComponent(const engine::Entity &entity, const std::string &name, std::function<void(T&)> drawFunc, const bool removable)
    {
        if(!entity.hasComponent<T>())
            return;

        auto &component = entity.getComponent<T>();
        if (ImGui::TreeNodeEx((void*) typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.c_str()))
        {
            drawFunc(component);

            if (removable)
            {
                ImGui::NewLine();

                if (ImGui::Button("Remove"))
                    entity.removeComponent<T>();
            }

            ImGui::TreePop();
        }
    }

    /*
     * Add Components
     */

    void InspectorPanel::drawAddComponentsPopup(const engine::Entity &entity)
    {
        ImGui::NewLine();
        if(ImGui::Button("Add Components"))
            ImGui::OpenPopup("AddComponents");

        if (!ImGui::BeginPopup("AddComponents"))
            return;

        //Transform
        drawAddComponent<TransformComponent>(entity, "Transform");

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
