#pragma once

namespace editor
{
    struct AccumulationResetEvent
    {
        unsigned int newFrameIndex = 1;
    };

    struct UpdateRaytracedObjectsBuffersEvent
    {
        bool resetAccumulation = true;
    };

    struct UpdateMeshesDataBuffersEvent
    {
        bool resetAccumulation = true;
    };

    struct PrimaryCameraMovedEvent
    {};
}
