#pragma once

#include <string>
#include <utility>
#include <imgui.h>
#include <imgui_internal.h>
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

        static bool Checkbox(const std::string &label, bool &value, const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 1);
            const bool changed = ImGui::Checkbox("##value", &value);

            EndColumnAlignedControl(disabledStyles);

            return changed;
        }

        static bool Drag1i(const std::string &label, int &value, const int speed = 1, const int min = 0, const int max = 0, const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 1);
            const bool changed = ImGui::DragInt("##value", &value, speed, min, max);

            EndColumnAlignedControl(disabledStyles);

            return changed;
        }

        static bool Drag1f(const std::string &label, float &value, const float speed = 1, const float min = 0, const float max = 0, const char *format = "%.3f", const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x + 1);
            const bool changed = ImGui::DragFloat("##value", &value, speed, min, max, format);

            EndColumnAlignedControl(disabledStyles);

            return changed;
        }

        static bool Drag3f(const std::string &label, glm::vec3 &values, const float speed = 1, const float min = 0, const float max = 0, const char *format = "%.3f", const float defaultValue = 0.0, const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

            const ImVec2 buttonSize = calculateButtonSize();

            bool changed = false;

            //X
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.1f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.7f, 0.15f, 0.15f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            if (ImGui::Button("X", buttonSize))
                values.x = defaultValue;
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &values.x, speed, min, max, format))
                changed = true;
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
            if (ImGui::DragFloat("##Y", &values.y, speed, min, max, format))
                changed = true;
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
            if (ImGui::DragFloat("##Z", &values.z, speed, min, max, format))
                changed = true;
            ImGui::PopItemWidth();

            EndColumnAlignedControl(disabledStyles);

            return changed;
        }

        static bool DragLinked3f(const std::string &label, glm::vec3 &values, bool &linked, const float speed = 1, const float min = 0, const float max = 0, const char *format = "%.3f", const float defaultValue = 0.0, const float columnWidth = 96.0f)
        {
            /*
             * This approach set the 2 axis to the modified axis if linked
             * The another way to do it would be to calculate the difference between old and new value and adding it to the other 2 axis.
             * Also, I hate imgui this is such a mess
             */

            //Modified version of BeginColumnAlignedControl
            ImGui::PushID(label.c_str());
            int disabledStyles = PushDisableRounding();

            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, columnWidth);
            ImGui::Text(label.c_str());

            ImGui::SameLine();
            ImGui::SetWindowFontScale(0.6f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (GImGui->FontSize + GImGui->Style.FramePadding.x * 2));
            ImGui::Checkbox("##Link", &linked);
            ImGui::SetWindowFontScale(1);

            ImGui::NextColumn();
            disabledStyles += PushDisableSpacing();

            //Modified version of Drag3f
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

            bool changed = false;

            const ImVec2 buttonSize = calculateButtonSize();

            //X
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.1f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.7f, 0.15f, 0.15f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            if (ImGui::Button("X", buttonSize))
            {
                if (linked)
                    values = glm::vec3(defaultValue);
                else
                    values.x = defaultValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &values.x, speed, min, max, format) && linked)
            {
                values.y = values.z = values.x;
                changed = true;
            }
            ImGui::PopItemWidth();
            ImGui::SameLine();

            //Y
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.8f, 0.1f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.15f, 0.7f, 0.15f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            if (ImGui::Button("Y", buttonSize))
            {
                if (linked)
                    values = glm::vec3(defaultValue);
                else
                    values.y = defaultValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &values.y, speed, min, max, format) && linked)
            {
                values.x = values.z = values.y;
                changed = true;
            }
            ImGui::PopItemWidth();
            ImGui::SameLine();

            //Z
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.1f, 0.8f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.15f, 0.15f, 0.7f, 1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            if (ImGui::Button("Z", buttonSize))
            {
                if (linked)
                    values = glm::vec3(defaultValue);
                else
                    values.z = defaultValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##Z", &values.z, speed, min, max, format) && linked)
            {
                values.x = values.y = values.z;
                changed = true;
            }
            ImGui::PopItemWidth();

            //Original EndColumnAlignedControl
            EndColumnAlignedControl(disabledStyles);

            return changed;
        }

        static bool Color3f(const std::string &label, glm::vec3 &value, const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            const bool changed = ImGui::ColorEdit3("##color", glm::value_ptr(value), ImGuiColorEditFlags_NoLabel);

            EndColumnAlignedControl(disabledStyles);

            return changed;
        }

        static bool InputText(const std::string &label, std::string &value, const float columnWidth = 96.0f)
        {
            const int disabledStyles = BeginColumnAlignedControl(label, columnWidth);

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            char buffer[128] = {};
            strcpy_s(buffer, value.c_str());

            bool changed = false;

            ImGui::PushID(label.c_str());
            if (ImGui::InputText("##text", buffer, sizeof(buffer)))
            {
                value = std::string(buffer);
                changed = true;
            }
            ImGui::PopID();

            EndColumnAlignedControl(disabledStyles);

            return changed;
        }
    };
}
