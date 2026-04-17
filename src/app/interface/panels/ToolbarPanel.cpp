#include "ToolbarPanel.h"

#include <portable-file-dialogs.h>
#if defined(interface) // Go fuck yourself Microsoft. ARE YOU SERIOUS??
    #undef interface
#endif

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "../../RaytracingApplication.h"

namespace application {
    void ToolbarPanel::draw()
    {
        if (!ImGui::BeginMainMenuBar())
            return;

        // File Category
        if (ImGui::BeginMenu("File"))
        {
            // Save Scene
            if (ImGui::MenuItem("Save Scene"))
            {
                pfd::save_file dialog("Select a File", ".", {"JSON", "*.json", "All Files", "*"});

                std::string path = dialog.result();
                if (path.find_last_of('.') == std::string::npos)
                    path.append(".json");

                if (!path.empty())
                    application->getSceneSerializer().serializeScene(application->getActiveScene(), path);
            }

            // Load Scene
            if (ImGui::MenuItem("Load Scene"))
            {
                pfd::open_file dialog("Select a File", ".", {"JSON", "*.json", "All Files", "*"});

                const std::vector<std::string> &paths = dialog.result();
                if (!paths.empty())
                {
                    try
                    {
                        engine::Scene *scene = application->getSceneSerializer().deserializeScene(paths[0], application->getGlobalMessenger());
                        application->loadScene(scene);
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

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
