#pragma once
#include "Singleton.h"
#include "config.h"

class Collider;

struct Ray {
	FPOINT origin;
	FPOINT direction; // 반드시 정규화된 방향
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
public:
	void Register(class Collider* collider);
	void UnRegister(class Collider* collider);

	
	void DrawRay(ID2D1RenderTarget* rt, FPOINT start, FPOINT dir, float length);

	bool RaycastAll(const Ray& ray, float maxDist, RaycastHit& hitOut, bool debugDraw = false, float debugTime = 0.0f);
	
private:
	void AddDebugRay(FPOINT origin, FPOINT direction, float length, float duration);
private:
	vector<Collider*> colliders;
	vector<DebugRay> debugRays;
};

