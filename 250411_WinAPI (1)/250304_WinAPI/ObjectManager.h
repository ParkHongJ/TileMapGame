#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>

class GameObject;

class ObjectManager : public Singleton<ObjectManager>
{
public:
    ObjectManager();
    virtual ~ObjectManager();

    void Init();
    void Update(float TimeDelta);
    void LateUpdate(float TimeDelta);
    void Render(ID2D1HwndRenderTarget* renderTarget);
    void Release();

    void AddObject(RENDERORDER renderId, GameObject* obj);
    void AddObject(unsigned int id, RENDERORDER renderId, GameObject* obj);
    void ReplaceObject(unsigned int id, GameObject* obj);
    bool RemoveObject(unsigned int id);

    GameObject* FindObject(unsigned int id);

private:
    unordered_map<unsigned int, GameObject*> objects;
    list<GameObject*> renders[RENDER_END];
    unsigned int nextId;
};

