#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>


class GameObject;
class InteractObject;
class Character;

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
    void Render(ID2D1RenderTarget* renderTarget);
    void Release();
    void ClearObjects();
    void AddObject(RENDERORDER renderId, GameObject* obj);
    void AddObject(unsigned int id, RENDERORDER renderId, GameObject* obj);
    void ReplaceObject(unsigned int id, GameObject* obj);
    bool RemoveObject(unsigned int id);

    Character* GetPlayer(void) { return player; };

    GameObject* FindObject(unsigned int id);
    GameObject* FindAbleInteractObject(GameObject* obj);

private:
    unordered_map<unsigned int, GameObject*> objects;
    Character* player;
    //unordered_map<unsigned int, GameObject*> tiles;
    //unordered_map<unsigned int, InteractObject*> interactObjects;
    list<GameObject*> renderObjects[RENDER_END];
    unsigned int nextId;
    //unsigned int nextTileId;
    //unsigned int nextInteractId;

};

