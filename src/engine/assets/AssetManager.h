#pragma once

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <utility>

namespace engine
{
    template<typename T>
    using AssetLoader = std::function<std::shared_ptr<T>(const std::string &path)>;

    template<typename T>
    class AssetManager
    {

    public:
        explicit AssetManager(std::string pathPrefix, AssetLoader<T> loader)
            : pathPrefix(std::move(pathPrefix)), loader(loader) {}

        std::shared_ptr<T> load(const std::string &path)
        {
            auto it = assets.find(path);
            if (it != assets.end())
            {
                if (std::shared_ptr<T> ref = it->second.lock())
                    return it->second.lock();
            }

            std::shared_ptr<T> ref = loader(pathPrefix + path);
            if (ref)
                assets.insert_or_assign(path, ref);

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
