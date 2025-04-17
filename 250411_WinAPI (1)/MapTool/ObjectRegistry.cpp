#include "ObjectRegistry.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <Windows.h>

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

std::string ObjectMetaLoader::WStringToString(const std::wstring& wstr)
{
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size_needed - 1, 0); // 마지막 널문자 제외
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);

    return result;
}
