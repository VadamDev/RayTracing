#include "RenderManager.h"

#include <spdlog/spdlog.h>

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
        renderMode = RenderMode::RENDER_ONLY;
    }

    void RenderManager::cancelRender()
    {
        renderMode = RenderMode::EDITOR;
    }

    void RenderManager::onFramePush()
    {
        if (data.frames++ > options.accumulatedFramesPerImage)
        {
            cancelRender();
            spdlog::info("Done rendering image!");
        }
    }
}
