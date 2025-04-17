#include "ObjectRegistry.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void ObjectRegistry::Register(const ObjectMeta& meta)
{
    _metas[meta.name] = meta;
}

const ObjectMeta* ObjectRegistry::Find(const std::string& name) const
{
    auto it = _metas.find(name);
    if (it != _metas.end())
        return &it->second;
    return nullptr;
}

const std::unordered_map<std::string, ObjectMeta>& ObjectRegistry::GetAll() const
{
    return _metas;
}

bool ObjectMetaLoader::LoadFromJson(const char* filePath)
{
    std::ifstream in(filePath);
    if (!in.is_open())
        return false;

    json data;
    in >> data;

    for (auto& item : data)
    {
        ObjectMeta meta;
        meta.name = item["name"].get<std::string>();
        std::string spath = item["atlasPath"].get<std::string>();
        meta.atlasPath = Utf8ToWstring(spath);
        meta.cellWidth = item["cellSize"]["x"].get<int>();
        meta.cellHeight = item["cellSize"]["y"].get<int>();

        ObjectRegistry::Get().Register(meta);
    }

    return true;
}
