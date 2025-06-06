#pragma once
#include "config.h"

class GameObject;
class CollisionManager;
struct Ray;
struct RaycastHit;

// WorldObject, Player, Monster, Item, Tile, Effect, Pushable

enum ORDER_Z
{
	Z_DEFAULT,
	Z_CAVE
};

enum class ColliderType
{
	NONE, 
	BOX,
	SPHERE
};

enum class CollisionMaskType : uint16_t
{
	WORLDOBJECT = 1,
	PLAYER = 1 << 1,
	MONSTER = 1 << 2,
	ITEM = 1 << 3,
	TILE = 1 << 4,
	EFFECT = 1 << 5,
	PLAYERATTACK = 1 << 6,
	PUSHABLE = 1 << 7, // 이게맞나?
};

class Collider
{
	friend CollisionManager;

public:
	Collider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject, ORDER_Z zOrder = Z_DEFAULT);
	Collider(FPOINT _Offset, FPOINT _Scale, CollisionMaskType maskType, GameObject* OwnerObject, ORDER_Z zOrder = Z_DEFAULT);
	virtual ~Collider();

	void Release() {};
	//부모의 위치를 기반으로 Offset과 등등을 갱신
public:
	virtual void Update(float TimeDelta) = 0;
	virtual bool CheckCollisionWithCircle(FPOINT center, float radius) const = 0;
	virtual bool CheckCollisionWithCircle(FPOINT center, float radius, float& distance) = 0;

public:
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const = 0;
private:
	virtual void DebugRender(ID2D1RenderTarget* rt);

	inline void SetScale(FPOINT scale) { Scale = scale; };

public:
	inline const FPOINT& GetWorldPos() { return Pos; }
	inline const FPOINT& GetScale() { return Scale; }
	inline const ColliderType& GetType() { return Type; }
	inline const CollisionMaskType& GetMaskType() { return maskType; }
	inline void SetMaskType(CollisionMaskType maskType) { this->maskType = maskType; };
	inline GameObject* GetOwner() const { return Owner; }

	inline void SetOffset(FPOINT offset) { this->Offset = offset; };

	inline ORDER_Z GetValueZ() const { return valueZ; }
	inline void SetValueZ(ORDER_Z zOrder) { valueZ = zOrder; }

protected:
	FPOINT Pos;
	FPOINT Offset;
	FPOINT Scale;
	GameObject* Owner;
	ColliderType Type;
	CollisionMaskType maskType;
	ORDER_Z valueZ;
};

class BoxCollider : public Collider
{
public:
	BoxCollider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject);
	BoxCollider(FPOINT _Offset, FPOINT _Scale, CollisionMaskType maskType, GameObject* OwnerObject);
	~BoxCollider() {};

public:
	virtual void Update(float TimeDelta) override;
	virtual void DebugRender(ID2D1RenderTarget* rt) override;

	virtual bool CheckCollisionWithCircle(FPOINT center, float radius) const override;
	virtual bool CheckCollisionWithCircle(FPOINT center, float radius, float& distance) override;
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const override;
	FPOINT Min, Max;
};

class SphereCollider : public Collider
{
public:
	SphereCollider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject, float radius);
	SphereCollider(FPOINT _Offset, FPOINT _Scale, CollisionMaskType maskType, GameObject* OwnerObject, float radius);
	~SphereCollider() {};

	virtual void Update(float TimeDelta) override;
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const override;

	virtual bool CheckCollisionWithCircle(FPOINT center, float radius) const override;
	virtual bool CheckCollisionWithCircle(FPOINT center, float radius, float& distance) override;

	float radius;
};