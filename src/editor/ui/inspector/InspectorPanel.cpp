#include "InspectorPanel.h"

#include "HierarchyPanel.h"
#include "../../scene/Components.h"
#include "../ImGuiUtils.hpp"
#include "../../../engine/messenger/Messenger.hpp"
#include "../../rendering/RenderingEvents.h"
#include "../../scene/SceneHandler.h"

namespace editor
{
    void InspectorPanel::draw(float deltaTime)
    {
        ImGui::Begin(getName());

        if (const engine::Entity &selectedEntity = hierarchyPanel->selectedEntity)
            drawComponents(selectedEntity);

        ImGui::End();
    }

    void InspectorPanel::drawComponents(const engine::Entity &entity)
    {
        // TagComponent
        if (entity.hasComponent<engine::TagComponent>())
        {
            std::string &tag = entity.getComponent<engine::TagComponent>().tag;

            char buffer[128] = {};
            strcpy_s(buffer, tag.c_str());

            ImGui::SetNextItemWidth(196);
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
                tag = std::string(buffer);

            ImGui::SameLine();
        }

        // Add components button
        drawAddComponentsPopup(entity);
        ImGui::NewLine();

        static bool linkedScaling = true;
        drawComponent<TransformComponent>(entity, "Transform", [this](TransformComponent &transform) {
            bool updBuffs = false;

            updBuffs |= Drag3f("Position", transform.position, 0.01f, 0, 0, "%.2f");
            updBuffs |= Drag3f("Rotation", transform.rotation, 0.1f, 0, 0, "%.1f");
            updBuffs |= DragLinked3f("Scale", transform.scale, linkedScaling, 0.01f, 0, std::numeric_limits<float>::infinity(), "%.2f");

            if (updBuffs)
                updateBuffers();
        });

        // Camera
        drawComponent<CameraComponent>(entity, "Camera", [this](CameraComponent &camera) {
            bool updBuffs = false;

            updBuffs |= Checkbox("Primary", camera.primary);
            updBuffs |= Drag1f("FOV", camera.fov, 1, 45, 130, "%.0f");
            updBuffs |= Drag1f("Focal Plane", camera.focalPlane, 0.01f, 1, 1e9f, "%.2f");

            if (updBuffs)
                updateBuffers();
        });

        // Raytraced Material
        drawComponent<RaytracedMaterialComponent>(entity, "Raytraced Material", [this](RaytracedMaterialComponent &component) {
            RaytracedMaterial &material = component.material;

            bool updBuffs = false;

            updBuffs |= Color3f("Color", material.color);
            updBuffs |= Drag1f("Smoothness", material.smoothness, 0.01f, 0, 1, "%.2f");

            ImGui::NewLine();

            updBuffs |= Color3f("Specular Color", material.specularColor);
            updBuffs |= Drag1f("Specular Probability", material.specularProbability, 0.01f, 0, 1, "%.2f");

            ImGui::NewLine();

            updBuffs |= Color3f("Emission Color", material.emissionColor);
            updBuffs |= Drag1f("Emission Strength", material.emissionStrength, 0.01f, 0, std::numeric_limits<float>::infinity(), "%.2f");

            ImGui::NewLine();

            updBuffs |= Drag1i("Material Type", material.type, 1, 0, 1);

            if (updBuffs)
                updateBuffers();
        });

        // Raytraced Sphere
        drawEmptyComponent<RaytracedSphereComponent>(entity, "Raytraced Sphere");

        // Raytraced Mesh
        drawComponent<RaytracedMeshComponent>(entity, "Raytraced Mesh", [this](RaytracedMeshComponent &mesh) {
            if (InputText("Mesh Name", mesh.name))
            {
                mesh.mesh = nullptr; // allows the RaytracedMeshSystem to retrieve the asset from the Model Manager (again)
                updateBuffers();
            }
        });
    }

    void InspectorPanel::drawAddComponentsPopup(const engine::Entity &entity)
    {
        if(ImGui::Button("Add Components"))
            ImGui::OpenPopup("AddComponents");

        if (!ImGui::BeginPopup("AddComponents"))
            return;

        // List every addeable component
        drawAddComponent<TransformComponent>(entity, "Transform");
        drawAddComponent<CameraComponent>(entity, "Camera");
        drawAddComponent<RaytracedMaterialComponent>(entity, "Raytraced Material");
        drawAddComponent<RaytracedSphereComponent>(entity, "Raytraced Sphere");
        drawAddComponent<RaytracedMeshComponent>(entity, "Raytraced Mesh");

        ImGui::EndPopup();
    }

    template<typename T, typename... Args>
    void InspectorPanel::drawAddComponent(const engine::Entity &entity, const std::string &name, Args &&... args)
    {
        beginDisableIf(entity.hasComponent<T>(), [entity, name, args...] {
            if (!ImGui::MenuItem(name.c_str()))
                return;

            entity.addComponent<T>(std::forward<Args>(args)...);
            ImGui::CloseCurrentPopup();
        });
    }

    static constexpr ImGuiTreeNodeFlags DEFAULT_COMPONENTS_FLAGS = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

    template<typename T>
    void InspectorPanel::drawComponent(const engine::Entity &entity, const std::string &name, std::function<void(T&)> drawFunc, const bool removable)
    {
        if(!entity.hasComponent<T>())
            return;

        const bool opened = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), DEFAULT_COMPONENTS_FLAGS, name.c_str());

        bool removed = false;
        if (removable)
        {
            ImGui::PushID(typeid(T).name());

            ImGui::SameLine(ImGui::GetWindowWidth() - 62);
            if (ImGui::Button("Remove"))
            {
                entity.removeComponent<T>();
                removed = true;
            }

            ImGui::PopID();
        }

        if (opened)
        {
            if (!removed)
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

    void InspectorPanel::updateBuffers() const
    {
        UpdateRaytracedObjectsBuffersEvent event;
        sceneHandler->getGlobalMessenger().dispatch(event);
    }
}
