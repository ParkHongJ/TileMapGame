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
	virtual bool CheckCollisionWithCircle(FPOINT center, float radius) const = 0;

public:
	inline const FPOINT& GetWorldPos() { return Pos; } //월드계산 전
	inline const FPOINT& GetScale() { return Scale; }
	inline const ColliderType& GetType() { return Type; }
public:
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const = 0;
private:
	virtual void DebugRender(ID2D1RenderTarget* rt);

	inline void SetScale(FPOINT scale) { Scale = scale; };

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

	virtual bool CheckCollisionWithCircle(FPOINT center, float radius) const override;
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

	bool CheckCollisionWithCircle(FPOINT center, float radius) const override;

	float radius;

};