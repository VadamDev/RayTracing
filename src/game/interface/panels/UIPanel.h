#pragma once

#include <string>
#include <utility>
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/vec3.hpp"

namespace application
{
    class UIPanel
    {

    public:
        explicit UIPanel(std::string name)
            : name(std::move(name)) {}
        virtual ~UIPanel() = default;

        virtual void draw() = 0;

        const char* getName() const { return name.c_str(); }

    protected:
        std::string name;

        /*
         * Styling
         */

        static int PushDisableRounding()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);

            return 3;
        }

        static int PushDisableSpacing()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            return 1;
        }

        static ImVec2 calculateButtonSize()
        {
            const float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2;
            return ImVec2(lineHeight + 3, lineHeight);
        }

        /*
         * Utility
         */

        static int BeginColumnAlignedControl(const std::string &label, const float columnWidth)
        {
            ImGui::PushID(label.c_str());
            const int disableStyles = PushDisableRounding();

            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, columnWidth);
            ImGui::Text(label.c_str());

            ImGui::NextColumn();
            return disableStyles + PushDisableSpacing();
        }

        static void EndColumnAlignedControl(const int disableStyles)
        {
            ImGui::Columns(1);
            ImGui::PopStyleVar(disableStyles);
            ImGui::PopID();
        }

        /*
         * Drag Floats
         */

        static void Drag1f(const std::string &label, float &value, const float speed = 1, const float min = 0, const float max = 0, const char *format = "%.3f", const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 1);
            ImGui::DragFloat("##value", &value, speed, min, max, format);

            EndColumnAlignedControl(disabledStyles);
        }

        static void Drag3f(const std::string &label, glm::vec3 &values, const float speed = 1, const float min = 0, const float max = 0, const char *format = "%.3f", const float defaultValue = 0.0, const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

            const ImVec2 buttonSize = calculateButtonSize();

            //X
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.1f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.7f, 0.15f, 0.15f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            if (ImGui::Button("X", buttonSize))
                values.x = defaultValue;
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##X", &values.x, speed, min, max, format);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            //Y
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.8f, 0.1f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.15f, 0.7f, 0.15f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            if (ImGui::Button("Y", buttonSize))
                values.y = defaultValue;
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##Y", &values.y, speed, min, max, format);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            //Z
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.1f, 0.8f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.15f, 0.15f, 0.7f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            if (ImGui::Button("Z", buttonSize))
                values.z = defaultValue;
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##Z", &values.z, speed, min, max, format);
            ImGui::PopItemWidth();

            EndColumnAlignedControl(disabledStyles);
        }

        static void Color4f(const std::string &label, glm::vec4 &value, const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::ColorEdit4("##color", glm::value_ptr(value), ImGuiColorEditFlags_NoLabel);

            EndColumnAlignedControl(disabledStyles);
        }
    };
}
