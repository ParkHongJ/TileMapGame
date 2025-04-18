#pragma once
#include "Singleton.h"
#include "config.h"

class Collider;
class GameObject;

struct Ray {
	FPOINT origin;
	FPOINT direction; // �ݵ�� ����ȭ�� ����
};

struct DebugRay
{
	FPOINT origin;
	FPOINT direction;
	float length;
	float remainingTime;

	DebugRay(FPOINT o, FPOINT d, float len, float duration)
		: origin(o), direction(d), length(len), remainingTime(duration) {
	}
};

struct RaycastHit {
	bool hit = false;
	FPOINT point;      // �浹 ����
	float distance = 0.0f;
	Collider* collider = nullptr;
};

class CollisionManager : public Singleton<CollisionManager>
{
public:
	void Init();
	void Update(float TimeDelta);
	void DebugRender(ID2D1HwndRenderTarget* renderTarget);
	void Release();
	void Clear();
public:
	void Register(class Collider* collider);
	void UnRegister(class Collider* collider);

	bool CollisionAABB(Collider* collider1, Collider* collider2);
	bool CollisionSphere(Collider* collider1, Collider* collider2);
	void BoxAll();
	
	bool RaycastAll(const Ray& ray, float maxDist, RaycastHit& hitOut, bool debugDraw = false, float debugTime = 0.0f, GameObject* ignoreObject = nullptr);
	bool GetObjectsInCircle(FPOINT center, float radius, vector<GameObject*>* inCircleObjects);
	bool GetObjectsInCircle(GameObject* owner, float radius, priority_queue<pair<float, GameObject*>>& inCircleObjects);
	bool GetInteractObjectsInCircle(GameObject* owner, float radius, priority_queue<pair<float, GameObject*>>& inCircleObjects);
	pair<GameObject*, GameObject*> GetInteractObjectPairInCircle(GameObject* owner, float radius);

private:
	void DrawRay(ID2D1RenderTarget* rt, FPOINT start, FPOINT dir, float length);
	void AddDebugRay(FPOINT origin, FPOINT direction, float length, float duration);

private:
	vector<Collider*> colliders;
	vector<DebugRay> debugRays;
};

