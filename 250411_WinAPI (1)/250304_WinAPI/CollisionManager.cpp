#include "CollisionManager.h"
#include "Collider.h"
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

void CollisionManager::AddDebugRay(FPOINT origin, FPOINT direction, float length, float duration)
{
    direction = direction.Normalized();
    debugRays.emplace_back(origin, direction, length, duration);
}
