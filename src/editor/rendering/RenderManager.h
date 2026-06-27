#pragma once

#include <string>
#include <chrono>

namespace engine
{
    class Messenger;
}

namespace editor
{
    class RenderingCanvas;

    enum class RenderMode
    {
        EDITOR, RENDER_ONLY
    };

    enum class ExportFormat
    {
        PNG, JPG, BMP, HDR
    };

    constexpr std::string_view exportFormatToExtension(const ExportFormat exportFormat)
    {
        switch (exportFormat)
        {
            case ExportFormat::PNG:
                return ".png";
            case ExportFormat::JPG:
                return ".jpg";
            case ExportFormat::BMP:
                return ".bmp";
            case ExportFormat::HDR:
                return ".hdr";
            default:
                return ".raw";
        }
    }

    struct RenderOptions
    {
        int accumulatedFramesPerImage = 16384;

        ExportFormat format = ExportFormat::PNG;
        std::string exportPath = "export";
        int jpgQuality = 80;
    };

    struct RenderData
    {
        int frames = 0;

        std::chrono::time_point<std::chrono::steady_clock> startTime;
    };

    class RenderManager
    {

    public:
        explicit RenderManager(engine::Messenger &globalMessenger, RenderingCanvas *canvas)
            : globalMessenger(globalMessenger), canvas(canvas) {}

        void beginRender(const RenderOptions &options);
        void cancelRender(bool exportImage = false);

        void onFramePush();

        RenderMode getRenderMode() const { return renderMode; }
        RenderOptions getRenderOptions() const { return options; }
        RenderData getRenderData() const { return data; }

    private:
        engine::Messenger &globalMessenger;
        RenderingCanvas *canvas;

        RenderMode renderMode = RenderMode::EDITOR;

        RenderOptions options;
        RenderData data;

        void exportCanvas() const;
    };
}
