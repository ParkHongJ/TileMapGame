#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>
#include <queue>

class GameObject;

enum RENDERORDER
{
    RENDER_BACKGROUND, RENDER_MONSTER, RENDER_ITEM, RENDER_PLAYER, RENDER_HOLD, RENDER_PLAYERCLIMB, RENDER_TILE, RENDER_UI, RENDER_END
};

enum INTRACTIVESTATE
{
    INTRACTIVE_ABLE, INTRACTIVE_NONE
};


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
    list<GameObject*> renderObjects[RENDER_END];
    priority_queue<pair<float, GameObject*>> interactObjects;
    unsigned int nextId;
};

