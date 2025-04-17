#include "CollisionManager.h"
#include "Collider.h"
#include "CommonFunction.h"

#include "GameObject.h"
void CollisionManager::Init()
{
}

void CollisionManager::Update(float TimeDelta)
{
	for (auto& collider : colliders)
	{
		collider->Update(TimeDelta);
	}

    for (int i = 0; i < debugRays.size(); )
    {
        debugRays[i].remainingTime -= TimeDelta;
        if (debugRays[i].remainingTime <= 0.f)
            debugRays.erase(debugRays.begin() + i);
        else
            ++i;
    }

    BoxAll();
}

void CollisionManager::Release()
{
	ReleaseInstance();
}

void CollisionManager::DebugRender(ID2D1HwndRenderTarget* renderTarget)
{
#ifdef _DEBUG
	for (auto& collider : colliders)
	{
		collider->DebugRender(renderTarget);
	}

    // Debug Ray 시각화
    for (const auto& ray : debugRays)
    {
        FPOINT end = {
            ray.origin.x + ray.direction.x * ray.length,
            ray.origin.y + ray.direction.y * ray.length
        };

        renderTarget->DrawLine(
            D2D1::Point2F(ray.origin.x, ray.origin.y),
            D2D1::Point2F(end.x, end.y),
            GBrush.Get(), // 또는 다른 전역 브러시
            1.5f
        );
    }
#endif
}

void CollisionManager::Register(Collider* collider)
{
	colliders.push_back(collider);
}

void CollisionManager::UnRegister(Collider* collider)
{

}

bool CollisionManager::CollisionAABB(Collider* collider1, Collider* collider2)
{
    FPOINT collider1Pos = collider1->GetWorldPos();
    FPOINT Collider1HalfSize = { collider1->GetScale().x * 0.5f,collider1->GetScale().y * 0.5f };

    FPOINT collider2Pos = collider2->GetWorldPos();
    FPOINT Collider2HalfSize = { collider2->GetScale().x * 0.5f,collider2->GetScale().y * 0.5f };

    RECT rc1;
    rc1.left = LONG(collider1Pos.x - Collider1HalfSize.x);
    rc1.right = LONG(collider1Pos.x + Collider1HalfSize.x);
    rc1.top = LONG(collider1Pos.y - Collider1HalfSize.y);
    rc1.bottom = LONG(collider1Pos.y + Collider1HalfSize.y);

    RECT rc2;
    rc2.left = LONG(collider2Pos.x - Collider2HalfSize.x);
    rc2.right = LONG(collider2Pos.x + Collider2HalfSize.x);
    rc2.top = LONG(collider2Pos.y - Collider2HalfSize.y);
    rc2.bottom = LONG(collider2Pos.y + Collider2HalfSize.y);

    return RectInRect(rc1, rc2);
}

bool CollisionManager::CollisionSphere(Collider* collider1, Collider* collider2)
{
    float radius = (collider1->GetScale().x + collider2->GetScale().x) * 0.5f;
    float distance = GetDistance(collider1->GetWorldPos(), collider2->GetWorldPos());

    return distance <= radius;
}

void CollisionManager::BoxAll()
{
    for (auto& iter : colliders)
    {
        for (auto& iter2 : colliders)
        {
            if (iter == iter2)
            {
                continue;
            }

            if (ColliderType::BOX != iter->GetType() && ColliderType::BOX != iter2->GetType())
            {
                continue;
            }

            bool bCollision = CollisionAABB(iter, iter2);

            if (bCollision)
            {
                int i = 5;
                // Test
            }
        }
    }
}

void CollisionManager::DrawRay(ID2D1RenderTarget* rt, FPOINT start, FPOINT dir, float length)
{
    FPOINT end = {
        start.x + dir.x * length,
        start.y + dir.y * length
    };

    rt->DrawLine(
        D2D1::Point2F(start.x, start.y),
        D2D1::Point2F(end.x, end.y),
        GBrush.Get(), 1.0f
    );
}

bool CollisionManager::RaycastAll(const Ray& ray, float maxDist, RaycastHit& outHit, bool debugDraw, float debugTime)
{
    bool found = false;
    RaycastHit closestHit;
    closestHit.distance = maxDist;

    for (auto* col : colliders)
    {
        if (!col) continue;

        RaycastHit temp;
        if (col->Raycast(ray, maxDist, temp))
        {
            if (temp.distance < closestHit.distance)
            {
                closestHit = temp;
                closestHit.collider = col;
                found = true;
            }
        }
    }

    if (found)
        outHit = closestHit;

    // 디버그 그리기
    if (debugDraw && debugTime > 0.0f)
    {
        float drawLen = found ? outHit.distance : maxDist;
        AddDebugRay(ray.origin, ray.direction, drawLen, debugTime);
    }

    return found;
}

bool CollisionManager::GetObjectsInCircle(FPOINT center, float radius, vector<GameObject*>* inCircleObjects)
{
    std::vector<GameObject*> result;

    float radiusSq = radius * radius;

    for (auto& col : colliders)
    {
        if (!col->CheckCollisionWithCircle(center, radius))
            continue;

        inCircleObjects->push_back(col->Owner);
    }

    if (inCircleObjects->empty())
        return false;

    return true;
}
void CollisionManager::AddDebugRay(FPOINT origin, FPOINT direction, float length, float duration)
{
    direction = direction.Normalized();
    debugRays.emplace_back(origin, direction, length, duration);
}
