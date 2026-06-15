#pragma once

#include <filesystem>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <utility>

namespace engine
{
    template<typename T>
    using AssetLoader = std::function<std::shared_ptr<T>(const std::filesystem::path &path)>;

    template<typename T>
    class AssetManager
    {

    public:
        explicit AssetManager(std::string pathPrefix, AssetLoader<T> loader)
            : pathPrefix(std::move(pathPrefix)), loader(loader) {}

        std::shared_ptr<T> load(const std::string &path)
        {
            std::filesystem::path sysPath(pathPrefix + path);
            const std::string &name = sysPath.stem().string();

            auto it = assets.find(name);
            if (std::shared_ptr<T> ref = it != assets.end() ? it->second.lock() : nullptr)
                return ref;

            std::shared_ptr<T> ref = loader(sysPath);
            if (ref)
                assets.insert_or_assign(name, ref);

            return ref;
        }

        bool isLoaded(const std::string &path)
        {
            return assets.contains(path);
        }

    protected:
        std::string pathPrefix;
        AssetLoader<T> loader;

        std::unordered_map<std::string, std::weak_ptr<T>> assets;
    };
}
