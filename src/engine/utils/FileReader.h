#pragma once

#include <fstream>
#include <spdlog/spdlog.h>

namespace engine::utils
{
    inline std::string readFile(const std::string &filePath)
    {
        const std::ifstream in(filePath);
        if (!in || !in.is_open())
        {
            spdlog::error("Failed to open file at path {}", filePath);
            return {};
        }

        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }
}
