#include "ToolbarPanel.h"

#include <portable-file-dialogs.h>
#if defined(interface) // GFY Microsoft, ARE YOU SERIOUS??
    #undef interface
#endif
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "../../scene/SceneHandler.h"

namespace editor
{
    void ToolbarPanel::draw()
    {
        if (!ImGui::BeginMainMenuBar())
            return;

        if (ImGui::BeginMenu("File"))
        {
            // Save Scene
            beginDisableIf(!sceneHandler->isSceneOpened(), [this] {
                if (ImGui::MenuItem("Save Scene"))
                {
                    pfd::save_file dialog("Select a destination", ".", { "JSON", "*.json", "All Files", "*" });

                    std::string path = dialog.result();
                    if (!path.empty())
                    {
                        // Append default file extension if no extension provided
                        if (path.find_last_of('.') == std::string::npos)
                            path.append(".json");

                        sceneHandler->saveCurrentScene(path);
                    }
                }
            });

            // Load Scene
            if (ImGui::MenuItem("Load Scene"))
            {
                pfd::open_file dialog("Select a scene file", ".", {"JSON", "*.json", "All Files", "*"});

                const std::vector<std::string> &paths = dialog.result();
                if (!paths.empty())
                {
                    try
                    {
                        sceneHandler->openScene(paths[0]);
                    }
                    catch (const nlohmann::detail::exception &e)
                    {
                        spdlog::error("Failed to parse scene json: {}", e.what());

                        pfd::message message("An error occurred", e.what(), pfd::choice::ok, pfd::icon::error);
                        message.result();
                    }
                    catch (const std::runtime_error &e)
                    {
                        spdlog::error("Failed to read scene json: {}", e.what());

                        pfd::message message("An error occurred", e.what(), pfd::choice::ok, pfd::icon::error);
                        message.result();
                    }
                }
            }

            // Create Empty Scene
            if (ImGui::MenuItem("New Empty Scene"))
            {
                sceneHandler->openNewEmptyScene();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
