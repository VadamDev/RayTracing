#include "RenderManager.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <filesystem>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "RenderingCanvas.h"
#include "RenderingEvents.h"
#include "../../engine/messenger/Messenger.hpp"

namespace editor
{
    void RenderManager::beginRender(const RenderOptions &options)
    {
        AccumulationResetEvent event;
        globalMessenger.dispatch(event);

        this->options = options;
        data = {};
        data.startTime = std::chrono::steady_clock::now();

        renderMode = RenderMode::RENDER_ONLY;
    }

    void RenderManager::cancelRender(const bool exportImage)
    {
        if (exportImage)
            exportCanvas();

        renderMode = RenderMode::EDITOR;
    }

    void RenderManager::onFramePush()
    {
        if (data.frames++ > options.accumulatedFramesPerImage)
            cancelRender(true);
    }

    void RenderManager::exportCanvas() const
    {
        std::filesystem::path exportPath(options.exportPath);
        if (!exportPath.has_extension())
            exportPath += exportFormatToExtension(options.format);

        const std::string pathStr = exportPath.string();
        const char* path = pathStr.c_str();

        const int width = canvas->getWidth();
        const int height = canvas->getHeight();

        stbi_flip_vertically_on_write(true);
        switch (options.format)
        {
            case ExportFormat::PNG:
                stbi_write_png(path, width, height, 4, canvas->exportTextureAs<unsigned char>(GL_UNSIGNED_BYTE).data(), width * 4);
                break;
            case ExportFormat::JPG:
                stbi_write_jpg(path, width, height, 4, canvas->exportTextureAs<unsigned char>(GL_UNSIGNED_BYTE).data(), options.jpgQuality);
                break;
            case ExportFormat::BMP:
                stbi_write_bmp(path, width, height, 4, canvas->exportTextureAs<unsigned char>(GL_UNSIGNED_BYTE).data());
                break;
            case ExportFormat::HDR:
                stbi_write_hdr(path, width, height, 4, canvas->exportTextureAs<float>(GL_FLOAT).data());
                break;
            default:
                throw std::runtime_error("Cannot export render to an unknown format!");
        }
    }
}
