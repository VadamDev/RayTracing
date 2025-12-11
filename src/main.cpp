#include <chrono>
#include <iostream>

#include "engine/window/Window.h"

using window::Window;
using std::chrono::high_resolution_clock;

int main()
{
    Window window(960, 540, "test");
    if (!window.create())
        return -1;

    std::cout << "Width: " << window.getWidth() << " | Height: " << window.getHeight() << std::endl;
    std::cout << window.getTitle() << std::endl;

    auto lastFrame = high_resolution_clock::now();

    int frames = 0;
    while (!window.shouldClose())
    {
        auto now = high_resolution_clock::now();

        if (now - lastFrame > std::chrono::seconds(1))
        {
            std::cout << "FPS: " << frames << std::endl;

            frames = 0;
            lastFrame = now;
        }

        window.update();
        frames++;
    }

    return 0;
}
