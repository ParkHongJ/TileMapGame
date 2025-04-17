#pragma once
#include <string>
#include <unordered_map>

class GameObject;

class IObjectCreator
{
public:
    virtual ~IObjectCreator() {}
    virtual GameObject* Create() const = 0;
};

template<typename T>
class ObjectCreator : public IObjectCreator
{
public:
    GameObject* Create() const override
    {
        return new T(); // ÇÙ½É
    }
};

class ObjectFactory
{
public:
    static ObjectFactory& Get();

    void Register(const std::string& name, IObjectCreator* creator);

    GameObject* Create(const std::string& name);

    ~ObjectFactory();

private:
    std::unordered_map<std::string, IObjectCreator*> creators;
};


template<typename T>
class ObjectRegistrar
{
public:
    ObjectRegistrar(const std::string& name)
    {
        ObjectFactory::Get().Register(name, new ObjectCreator<T>());
    }
};

#define REGISTER_OBJECT(CLASS_NAME) \
    static ObjectRegistrar<CLASS_NAME> _reg_##CLASS_NAME(#CLASS_NAME);