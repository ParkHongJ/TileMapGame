#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>

class GameObject;
class ObjectManager : public Singleton<ObjectManager>
{
public:
    ObjectManager();

    void Init();

    void AddObject(GameObject* obj);

    void RemoveObject(int id);

    void Render(ID2D1HwndRenderTarget* renderTarget);

    void Update(float TimeDelta);

    std::shared_ptr<GameObject> FindObject(int id);

    void Release();

private:
    unordered_map<int, GameObject*> m_objects;

    unsigned int nextId = 0;

};

