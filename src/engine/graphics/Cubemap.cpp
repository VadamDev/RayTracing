#include "Cubemap.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <spdlog/spdlog.h>

namespace engine
{
    Cubemap::Cubemap(const int width, const int height, const std::vector<std::string> &faces)
        : width(width), height(height), faces(faces)
    {
        if (faces.size() != 6)
            throw std::runtime_error("A cubemap must have 6 faces");
    }

    Cubemap::~Cubemap()
    {
        if (textureHandle != 0)
            glDeleteTextures(1, &textureHandle);
    }

    void Cubemap::create()
    {
        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);

        for (int i = 0; i < faces.size(); i++)
        {
            void *pixels = stbi_load(faces[i].c_str(), &width, &height, nullptr, 3);
            if (!pixels)
                spdlog::error("Failed to load cubemap face: {}", faces[i]);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            stbi_image_free(pixels);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void Cubemap::bind() const
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);
    }

    void Cubemap::unbind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}
