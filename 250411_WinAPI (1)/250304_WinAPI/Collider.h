#pragma once
#include "config.h"

class GameObject;
class CollisionManager;
struct Ray;
struct RaycastHit;
enum class ColliderType
{
	NONE, 
	BOX,
	SPHERE
};
class Collider
{
	friend CollisionManager;

public:
	Collider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject);
	virtual ~Collider();

	void Release() {};
	//부모의 위치를 기반으로 Offset과 등등을 갱신
public:
	virtual void Update(float TimeDelta) = 0;
	virtual void DebugRender(ID2D1RenderTarget* rt);
public:
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const = 0;

protected:
	FPOINT Pos;
	FPOINT Offset;
	FPOINT Scale;
	GameObject* Owner;
	ColliderType Type;
};

class BoxCollider : public Collider
{
public:
	BoxCollider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject);
	~BoxCollider() {};

public:
	virtual void Update(float TimeDelta) override;
	virtual void DebugRender(ID2D1RenderTarget* rt) override;
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const override;
	FPOINT Min, Max;
};

class SphereCollider : public Collider
{
public:
	SphereCollider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject, float radius);
	~SphereCollider() {};

	virtual void Update(float TimeDelta) override;
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const override;
	float Radius;

};