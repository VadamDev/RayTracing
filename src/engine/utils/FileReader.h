#pragma once

#include <fstream>
#include <spdlog/spdlog.h>

namespace engine::utils {
    static std::string readFile(const std::string &filePath)
    {
        std::ifstream in;
        in.open(filePath);

        std::string result;
        if (!in.is_open())
        {
            spdlog::error("Failed to open file at path {}", filePath);
            return result;
        }

        std::string line;
        while(getline(in, line))
            result.append(line).append("\n");

        return result;
    }
}

