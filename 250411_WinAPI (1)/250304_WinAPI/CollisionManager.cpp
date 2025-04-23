#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "CommonFunction.h"

#include "GameObject.h"
//임시
#include "CameraManager.h"

//enum class CollisionMaskType : uint8_t
//{
//    WORLDOBJECT = 1,
//    PLAYER = 1 << 1,
//    MONSTER = 1 << 2,
//    ITEM = 1 << 3,
//    TILE = 1 << 4,
//    EFFECT = 1 << 5
//};
void CollisionManager::Init()
{
    layerMaskMap[CollisionMaskType::WORLDOBJECT] = uint16_t(CollisionMaskType::PLAYER) |
        uint16_t(CollisionMaskType::MONSTER) | uint16_t(CollisionMaskType::TILE);

    layerMaskMap[CollisionMaskType::PLAYER] = uint16_t(CollisionMaskType::WORLDOBJECT) |
        uint16_t(CollisionMaskType::MONSTER) | uint16_t(CollisionMaskType::ITEM) |
        uint16_t(CollisionMaskType::TILE);

    layerMaskMap[CollisionMaskType::MONSTER] = uint16_t(CollisionMaskType::PLAYER) |
        uint16_t(CollisionMaskType::WORLDOBJECT) | 
        uint16_t(CollisionMaskType::TILE);

    layerMaskMap[CollisionMaskType::ITEM]/* = uint8_t(CollisionMaskType::PLAYER)*/;

    layerMaskMap[CollisionMaskType::TILE] = uint16_t(CollisionMaskType::PLAYER)
        | uint16_t(CollisionMaskType::MONSTER) || uint16_t(CollisionMaskType::EFFECT);

    layerMaskMap[CollisionMaskType::EFFECT] = uint16_t(CollisionMaskType::TILE);

    layerMaskMap[CollisionMaskType::PLAYERATTACK] = 
         uint16_t(CollisionMaskType::WORLDOBJECT) | uint16_t(CollisionMaskType::MONSTER);

    //Ray
    layerRayMaskMap[CollisionMaskType::WORLDOBJECT] = uint16_t(CollisionMaskType::PLAYER) |
        uint16_t(CollisionMaskType::MONSTER) /*| uint16_t(CollisionMaskType::TILE)*/;

    layerRayMaskMap[CollisionMaskType::PLAYER] = 
        uint16_t(CollisionMaskType::ITEM) | uint16_t(CollisionMaskType::TILE); // Player로 테스트

    layerRayMaskMap[CollisionMaskType::MONSTER] = uint16_t(CollisionMaskType::PLAYER) |
        uint16_t(CollisionMaskType::WORLDOBJECT) | uint16_t(CollisionMaskType::TILE);

    layerRayMaskMap[CollisionMaskType::ITEM] = uint16_t(CollisionMaskType::PLAYER) | uint16_t(CollisionMaskType::TILE);

    layerRayMaskMap[CollisionMaskType::TILE] = uint16_t(CollisionMaskType::PLAYER)
        | uint16_t(CollisionMaskType::MONSTER) || uint16_t(CollisionMaskType::EFFECT);

    layerRayMaskMap[CollisionMaskType::EFFECT] = uint16_t(CollisionMaskType::TILE);

    layerRayMaskMap[CollisionMaskType::PLAYERATTACK] =
        uint16_t(CollisionMaskType::WORLDOBJECT) | uint16_t(CollisionMaskType::MONSTER);
    
}

void CollisionManager::Update(float TimeDelta)
{
    for (auto& test : layerCollisionMap)
    {
        for (auto iter = test.second.begin(); iter != test.second.end();)
        {
            if (true == (*iter)->GetOwner()->IsDestroyed())
            {
                iter = test.second.erase(iter);
            }

            else
            {
                (*iter)->Update(TimeDelta);
                ++iter;
            }
        }
    }

    for (int i = 0; i < debugRays.size(); )
    {
        debugRays[i].remainingTime -= TimeDelta;
        if (debugRays[i].remainingTime <= 0.f)
            debugRays.erase(debugRays.begin() + i);
        else
            ++i;
    }

    //BoxAll();
    ColMaskAABB();
}

void CollisionManager::Release()
{
    colliders.clear();

    for (auto& colliderSet : layerCollisionMap)
    {
        colliderSet.second.clear();
    }

	//ReleaseInstance();
}

void CollisionManager::Clear()
{
    colliders.clear();

    for (auto& colliderSet : layerCollisionMap)
    {
        colliderSet.second.clear();
    }
}

void CollisionManager::DebugRender(ID2D1HwndRenderTarget* renderTarget)
{
#ifdef _DEBUG
	for (auto& collider : colliders)
	{
		collider->DebugRender(renderTarget);
	}

    for (auto& colliderSet : layerCollisionMap)
    {
        for (auto iter : colliderSet.second)
        {
            iter->DebugRender(renderTarget);
        }
    }

    // Debug Ray 시각화
    for (const auto& ray : debugRays)
    {
        FPOINT cameraPos = CameraManager::GetInstance()->GetPos();
        FPOINT end = {
            ray.origin.x + ray.direction.x * ray.length + cameraPos.x,
            ray.origin.y + ray.direction.y * ray.length + cameraPos.y
        };

        renderTarget->DrawLine(
            D2D1::Point2F(ray.origin.x + cameraPos.x, ray.origin.y + cameraPos.y),
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

void CollisionManager::RegisterMask(Collider* collider)
{
    layerCollisionMap[collider->GetMaskType()].insert(collider);
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
                iter->GetOwner()->Detect(iter2->GetOwner());
                iter2->GetOwner()->Detect(iter->GetOwner());
                // Test
            }
        }
    }
}

void CollisionManager::ColMaskAABB()
{
   // set<
    set<pair<CollisionMaskType, CollisionMaskType>> Set; // 되려나

    for (const auto& temp : layerCollisionMap)
    {
        uint16_t mask = layerMaskMap[temp.first];

        for (const auto& dest : layerCollisionMap)
        {
            // 검사 해야하는 레이어라면

            //if (uint16_t(pair.first) & uint16_t(pair2.first))
            //{
            //    continue;
            //}

            uint16_t type = uint16_t(dest.first); 
            
            if (mask & type)
            {
                
                if (temp.first < dest.first)
                {
                    if (Set.end() == Set.find({ temp.first, dest.first }))
                    {
                        Set.emplace(temp.first, dest.first);
                    }

                    else
                    {
                        continue;
                    }
                    
                }

                else
                {
					if (Set.end() == Set.find({ dest.first, temp.first }))
					{
						Set.emplace(dest.first, temp.first);
					}

					else
					{
						continue;
					}
                }
               
                //Set.
                for (auto& iter : temp.second)
                {
                    for (auto& iter2 : dest.second)
                    {
                        // z가 다르다고?
                        if (iter->GetValueZ() != iter2->GetValueZ())
                        {
                            continue;
                        }

                        bool bCollision = CollisionAABB(iter, iter2);

                        if (bCollision)
                        {
                            iter->GetOwner()->Detect(iter2->GetOwner());
                            iter2->GetOwner()->Detect(iter->GetOwner());
                        }
                    }
                }
            }
        }
    }
}

bool CollisionManager::ChangeMaskType(CollisionMaskType curMaskType, CollisionMaskType nextMaskType, GameObject* owner)
{
    if (curMaskType == nextMaskType)
    {
        return false;
    }

    for (auto& iter : layerCollisionMap[curMaskType]) // 비효율적
    {
        if (owner == iter->GetOwner())
        {
            iter->SetMaskType(nextMaskType);
            layerCollisionMap[nextMaskType].insert(iter);
            layerCollisionMap[curMaskType].erase(iter);
            return true;
        }
    }

    return false;

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

bool CollisionManager::RaycastAll(const Ray& ray, float maxDist, RaycastHit& outHit, bool debugDraw, float debugTime, GameObject* ignoreObject)
{
    // Don't use this function
    bool found = false;
    RaycastHit closestHit;
    closestHit.distance = maxDist;

    for (auto* col : colliders)
    {
        if (!col) continue;

        if (col->Owner == nullptr) continue;

        if (col->Owner == ignoreObject)
        {
            continue;
        }
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

bool CollisionManager::RaycastType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, bool debugDraw, float debugTime, ORDER_Z zOrder)
{
    bool found = false;
    RaycastHit closestHit;
    closestHit.distance = maxDist;

    for (auto* col : layerCollisionMap[maskType])
    {
        if (!col) continue;

        if (col->Owner == nullptr) continue;

        if (zOrder != col->GetValueZ())
        {
            continue;
        }

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
        hitOut = closestHit;

    // 디버그 그리기
    if (debugDraw && debugTime > 0.0f)
    {
        float drawLen = found ? hitOut.distance : maxDist;
        AddDebugRay(ray.origin, ray.direction, drawLen, debugTime);
    }

    return found;
}

bool CollisionManager::RaycastType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, GameObject* obj, bool debugDraw, float debugTime, ORDER_Z zOrder)
{
    bool found = false;
    RaycastHit closestHit;
    closestHit.distance = maxDist;
    GameObject* dest = nullptr;

    for (auto* col : layerCollisionMap[maskType])
    {
        if (!col) continue;

        if (col->Owner == nullptr) continue;

        if (zOrder != col->GetValueZ())
        {
            continue;
        }

        RaycastHit temp;
        if (col->Raycast(ray, maxDist, temp))
        {
            if (temp.distance < closestHit.distance)
            {
                closestHit = temp;
                closestHit.collider = col;
                found = true;
                dest = col->Owner;
            }
        }
    }

    if (found)
    {
        hitOut = closestHit;
        obj->Detect(dest);
        dest->Detect(obj);
    }
        

    // 디버그 그리기
    if (debugDraw && debugTime > 0.0f)
    {
        float drawLen = found ? hitOut.distance : maxDist;
        AddDebugRay(ray.origin, ray.direction, drawLen, debugTime);
    }

    return found;

    return false;
}

bool CollisionManager::RaycastMyType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, bool debugDraw, float debugTime, ORDER_Z zOrder)
{
    bool found = false;
    RaycastHit closestHit;
    closestHit.distance = maxDist;

    uint16_t mask = layerRayMaskMap[maskType];

	for (const auto& pair : layerCollisionMap)
	{
        uint16_t type = uint16_t(pair.first);
		// 검사 해야하는 레이어라면
		if (mask & type)
		{
			for (auto& iter : layerCollisionMap[pair.first])
			{
				if(zOrder == iter->GetValueZ())
				{
					RaycastHit temp;
					if (iter->Raycast(ray, maxDist, temp))
					{
						if (temp.distance < closestHit.distance)
						{
							closestHit = temp;
							closestHit.collider = iter;
							found = true;
						}
					}
				}
			}
		}
	}

    if (found)
        hitOut = closestHit;

    // 디버그 그리기
    if (debugDraw && debugTime > 0.0f)
    {
        float drawLen = found ? hitOut.distance : maxDist;
        AddDebugRay(ray.origin, ray.direction, drawLen, debugTime);
    }

    return found;
}

bool CollisionManager::RaycastMyType(const Ray& ray, float maxDist, RaycastHit& hitOut, CollisionMaskType maskType, GameObject* obj, bool debugDraw, float debugTime, ORDER_Z zOrder)
{
    bool found = false;
    RaycastHit closestHit;
    closestHit.distance = maxDist;
    GameObject* dest = nullptr;

    uint16_t mask = layerRayMaskMap[maskType];

    for (const auto& pair : layerCollisionMap)
    {
        uint16_t type = uint16_t(pair.first);
        // 검사 해야하는 레이어라면
        if (mask & type)
        {
            for (auto& iter : layerCollisionMap[pair.first])
            {
				RaycastHit temp;
                if (zOrder != iter->GetValueZ())
                {
                    continue;
                }

				if (iter->Raycast(ray, maxDist, temp))
				{
					if (temp.distance < closestHit.distance)
					{
						closestHit = temp;
						closestHit.collider = iter;
						found = true;
                        dest = iter->Owner;
					}
				}
            }
        }
    }

    if (found)
    {
        hitOut = closestHit;
        dest->Detect(obj);
        obj->Detect(dest);
    }

    // 디버그 그리기
    if (debugDraw && debugTime > 0.0f)
    {
        float drawLen = found ? hitOut.distance : maxDist;
        AddDebugRay(ray.origin, ray.direction, drawLen, debugTime);
    }

    return found;
}

bool CollisionManager::GetObjectsInCircle(FPOINT center, float radius, vector<GameObject*>* inCircleObjects)
{
    // std::vector<GameObject*> result; ?
    
    float radiusSq = radius * radius;

    for (auto& col : colliders)
    {
        if (!col->CheckCollisionWithCircle(center, radius))
            continue;

        inCircleObjects->push_back(col->Owner);
    }

    for (auto& col : layerCollisionMap)
    {
        for (auto& iter : col.second)
        {
            if (!iter->CheckCollisionWithCircle(center, radius))
                continue;

            inCircleObjects->push_back(iter->Owner);
        }

    }

    if (inCircleObjects->empty())
        return false;

    return true;
}
bool CollisionManager::GetObjectsInCircle(GameObject* owner, float radius, priority_queue<pair<float, GameObject*>>& inCircleObjects)
{
    FPOINT center = owner->GetPos();

    for (auto& col : colliders)
    {
        if (owner == (col->GetOwner()))
        {
            continue;
        }

        float distance = 0.f;

        if (!col->CheckCollisionWithCircle(center, radius, distance))
            continue;

        inCircleObjects.push({ distance, col->Owner });
    }

    return !inCircleObjects.empty();
}

bool CollisionManager::GetInteractObjectsInCircle(GameObject* owner, float radius, priority_queue<pair<float, GameObject*>>& inCircleObjects)
{
    FPOINT center = owner->GetPos();

 /*   for (auto& col : colliders)
    {
        if (owner == (col->GetOwner()) || INTERACTSTATE::INTERACT_ABLE != col->GetOwner()->GetObjectInteractState())
        {
            continue;
        }

        float distance = 0.f;

        if (!col->CheckCollisionWithCircle(center, radius, distance))
            continue;

        inCircleObjects.push({ distance, col->Owner });
    }*/

    for (auto& col : layerCollisionMap)
    {
        for (auto& iter : col.second)
        {
            if (owner == (iter->GetOwner()))
            {
                continue;
            }

            if (INTERACTSTATE::INTERACT_ABLE != iter->GetOwner()->GetObjectInteractState())
            {
                continue;
            }

            float distance = 0.f;

            if (!iter->CheckCollisionWithCircle(center, radius, distance))
                continue;

            inCircleObjects.push({ -distance, iter->Owner });
        }
    }

    return !inCircleObjects.empty();
}

pair<GameObject*, GameObject*> CollisionManager::GetInteractObjectPairInCircle(GameObject* owner, float radius)
{
    FPOINT center = owner->GetPos();
    priority_queue<pair<float, GameObject*>> inCircleObjects;

    for (auto& col : colliders)
    {
        if (owner == (col->GetOwner()) || INTERACTSTATE::INTERACT_ABLE != col->GetOwner()->GetObjectInteractState())
        {
            continue;
        }

        float distance = 0.f;

        if (!col->CheckCollisionWithCircle(center, radius, distance))
            continue;

        inCircleObjects.push({ distance, col->Owner });
    }
    
    if (!inCircleObjects.empty())
    {
        // 우선순위 판단 후 pair 정렬?
        return { owner, inCircleObjects.top().second };
    }

    return pair<GameObject*, GameObject*>(nullptr, nullptr);
}
void CollisionManager::AddDebugRay(FPOINT origin, FPOINT direction, float length, float duration)
{
    direction = direction.Normalized();
    debugRays.emplace_back(origin, direction, length, duration);
}
