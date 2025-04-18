#pragma once
#include "ObjectMeta.h"
#include <unordered_map>
#include <fstream>
#include <string>
#include <codecvt>
#include <locale>

class ObjectMetaLoader
{
public:
    static bool LoadFromJson(const char* filePath);

    static std::wstring Utf8ToWstring(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.from_bytes(str);
    }
    static std::string WStringToString(const std::wstring& wstr);
};

class ObjectRegistry
{
public:
    static ObjectRegistry& Get()
    {
        static ObjectRegistry instance;
        return instance;
    }

    void Register(const ObjectMeta& meta);
    const ObjectMeta* Find(const std::string& name) const;
    const std::unordered_map<std::string, ObjectMeta>& GetAll() const;

   

private:
    std::unordered_map<std::string, ObjectMeta> _metas;
};