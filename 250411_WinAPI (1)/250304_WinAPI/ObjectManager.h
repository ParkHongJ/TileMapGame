#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>


class GameObject;
class InteractObject;

enum RENDERORDER
{
    RENDER_BACKGROUND, RENDER_MONSTER, RENDER_ITEM, RENDER_PLAYER, RENDER_HOLD, RENDER_PLAYERCLIMB, RENDER_TILE, RENDER_UI, RENDER_END
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
    GameObject* FindAbleInteractObject(GameObject* obj);

private:
    unordered_map<unsigned int, GameObject*> objects;
    //unordered_map<unsigned int, GameObject*> tiles;
    //unordered_map<unsigned int, InteractObject*> interactObjects;
    list<GameObject*> renderObjects[RENDER_END];
    unsigned int nextId;
    //unsigned int nextTileId;
    //unsigned int nextInteractId;
};

