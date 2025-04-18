#include "pch.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "CommonFunction.h"
//임시
#include "CameraManager.h"

Collider::Collider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject)
	: Offset(_Offset), Scale(_Scale), Owner(OwnerObject), Pos({ 0.f, 0.f })
{
	//순서보장 확실히 필요하다.
	CollisionManager::GetInstance()->Register(this);
}

Collider::~Collider()
{
	CollisionManager::GetInstance()->UnRegister(this);
}

void Collider::DebugRender(ID2D1RenderTarget* rt)
{
	//rt->DrawRectangle(worldRect, brush, 1.0f);
}

BoxCollider::BoxCollider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject)
	: Collider(_Offset, _Scale, OwnerObject)
	, Min({ 0.f, 0.f })
	, Max({ 0.f, 0.f })
{
    Type = ColliderType::BOX;
}

void BoxCollider::Update(float TimeDelta)
{
	//부모의 위치를 기반으로 min max 재계산

	FPOINT worldPos = {
		Owner->GetPos().x + Offset.x,
		Owner->GetPos().y + Offset.y
	};
	Pos = worldPos;
	float halfW = Scale.x * 0.5f;
	float halfH = Scale.y * 0.5f;

	Min = { worldPos.x - halfW, worldPos.y - halfH };
	Max = { worldPos.x + halfW, worldPos.y + halfH };

	/*worldRect = D2D1::RectF(
		worldPos.x - halfW,
		worldPos.y - halfH,
		worldPos.x + halfW,
		worldPos.y + halfH
	);*/
}

void BoxCollider::DebugRender(ID2D1RenderTarget* rt)
{
    //임시 코드
    FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();

	DrawCenteredRect(rt, cameraPos, Scale.x / 2.f, D2D1::ColorF(D2D1::ColorF::Green));
}

bool BoxCollider::CheckCollisionWithCircle(FPOINT center, float radius) const
{
    float closestX = Clamp<float>(center.x, Min.x, Max.x);
    float closestY = Clamp<float>(center.y, Min.y, Max.y);

    float dx = center.x - closestX;
    float dy = center.y - closestY;

    return (dx * dx + dy * dy) <= radius * radius;
}

bool BoxCollider::CheckCollisionWithCircle(FPOINT center, float radius, float& distance)
{
    float closestX = Clamp<float>(center.x, Min.x, Max.x);
    float closestY = Clamp<float>(center.y, Min.y, Max.y);

    float dx = center.x - closestX;
    float dy = center.y - closestY;
    distance = sqrtf(dx * dx + dy * dy);
    return distance <= radius;
}

bool BoxCollider::Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const
{
    float tmin = 0.0f;
    float tmax = maxDistance;

    FRECT rect = { Min.x, Min.y, Max.x, Max.y };
    // X축
    if (ray.direction.x == 0.0f)
    {
        if (ray.origin.x < rect.left || ray.origin.x > rect.right)
            return false;
    }
    else
    {
        float tx1 = (rect.left - ray.origin.x) / ray.direction.x;
        float tx2 = (rect.right - ray.origin.x) / ray.direction.x;

        tmin = max(tmin, min(tx1, tx2));
        tmax = min(tmax, max(tx1, tx2));
    }

    // Y축
    if (ray.direction.y == 0.0f)
    {
        if (ray.origin.y < rect.top || ray.origin.y > rect.bottom)
            return false;
    }
    else
    {
        float ty1 = (rect.top - ray.origin.y) / ray.direction.y;
        float ty2 = (rect.bottom - ray.origin.y) / ray.direction.y;

        tmin = max(tmin, min(ty1, ty2));
        tmax = min(tmax, max(ty1, ty2));
    }

    // 최종 교차 판정
    if (tmax >= tmin && tmin >= 0.0f && tmin <= maxDistance)
    {
        outHit.hit = true;
        outHit.distance = tmin;
        outHit.point = {
            ray.origin.x + ray.direction.x * tmin,
            ray.origin.y + ray.direction.y * tmin
        };
        return true;
    }

    return false;
}

SphereCollider::SphereCollider(FPOINT _Offset, FPOINT _Scale, GameObject* OwnerObject, float radius)
	: Collider(_Offset, _Scale, OwnerObject)
	, radius(radius)
{
    Type = ColliderType::SPHERE;
}

void SphereCollider::Update(float TimeDelta)
{
}

bool SphereCollider::Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit) const
{
	return false;
}

bool SphereCollider::CheckCollisionWithCircle(FPOINT center, float radius) const
{
    float totalR = radius + this->radius;
    return (Pos - center).Length() <= totalR * totalR;
}

bool SphereCollider::CheckCollisionWithCircle(FPOINT center, float radius, float& distance)
{
    float totalR = radius + this->radius;
    distance = (Pos - center).Length();
    return distance <= totalR * totalR;
}
