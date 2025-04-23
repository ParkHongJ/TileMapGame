#pragma once
#include "Singleton.h"
#include "config.h"
#include "Collider.h"

class GameObject;

struct Ray {
	FPOINT origin;
	FPOINT direction; // 반드시 정규화된 방향
};

//namespace std {
//	template <>
//	struct hash<CollisionMaskType> {
//		size_t operator()(const CollisionMaskType& type) const {
//			return static_cast<size_t>(type);
//		}
//	};

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
	FPOINT point;      // 충돌 지점
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
	void RegisterMask(class Collider* collider);
	void UnRegister(class Collider* collider);

	bool CollisionAABB(Collider* collider1, Collider* collider2);
	bool CollisionSphere(Collider* collider1, Collider* collider2);
	void BoxAll();
	bool RaycastAll(const Ray& ray, float maxDist, RaycastHit& hitOut, bool debugDraw = false, float debugTime = 0.0f, GameObject* ignoreObject = nullptr);
	bool RaycastType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, bool debugDraw = false, float debugTime = 0.0f, ORDER_Z zOrder = Z_DEFAULT);
	bool RaycastType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, GameObject* obj, bool debugDraw = false, float debugTime = 0.0f, ORDER_Z zOrder = Z_DEFAULT);

	bool RaycastMyType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, bool debugDraw = false, float debugTime = 0.0f, ORDER_Z zOrder = Z_DEFAULT);
	bool RaycastMyType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, GameObject* obj, bool debugDraw = false, float debugTime = 0.0f, ORDER_Z zOrder = Z_DEFAULT);

	bool GetObjectsInCircle(FPOINT center, float radius, vector<GameObject*>* inCircleObjects);
	bool GetObjectsInCircle(GameObject* owner, float radius, priority_queue<pair<float, GameObject*>>& inCircleObjects);
	bool GetInteractObjectsInCircle(GameObject* owner, float radius, priority_queue<pair<float, GameObject*>>& inCircleObjects);
	pair<GameObject*, GameObject*> GetInteractObjectPairInCircle(GameObject* owner, float radius);
	void ColMaskAABB();

	bool ChangeMaskType(CollisionMaskType curMaskType, CollisionMaskType nextMaskType, GameObject* owner);


private:
	void DrawRay(ID2D1RenderTarget* rt, FPOINT start, FPOINT dir, float length);
	void AddDebugRay(FPOINT origin, FPOINT direction, float length, float duration);

private:
	unordered_map<CollisionMaskType, unordered_set<Collider*>> layerCollisionMap;
	unordered_map<CollisionMaskType, uint16_t> layerMaskMap;
	unordered_map<CollisionMaskType, uint16_t> layerRayMaskMap;

	vector<Collider*> colliders;
	vector<DebugRay> debugRays;
};

