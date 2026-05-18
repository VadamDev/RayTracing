#include "TestPanel.h"

#include <imgui.h>

namespace editor
{
    void TestPanel::draw()
    {
        ImGui::Begin(getName());

        ImGui::Text(std::format("{}", clock->getRenderProfiler()->getSpentTimeAvgMs()).c_str());

        ImGui::End();
    }
}
