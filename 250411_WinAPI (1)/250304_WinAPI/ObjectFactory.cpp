#include "pch.h"
#include "ObjectFactory.h"

ObjectFactory& ObjectFactory::Get()
{
    static ObjectFactory instance;
    return instance;
}

void ObjectFactory::Register(const std::string& name, IObjectCreator* creator)
{
    creators[name] = creator;
}


GameObject* ObjectFactory::Create(const std::string& name)
{
    auto it = creators.find(name);
    if (it != creators.end())
        return it->second->Create();
    return nullptr;
}

ObjectFactory::~ObjectFactory()
{
    for (auto it = creators.begin(); it != creators.end(); ++it)
    {
        delete it->second;
    }
}
