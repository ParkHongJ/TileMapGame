#include "pch.h"
#include "GameObject.h"
#include "CameraManager.h"

HRESULT GameObject::Init()
{
	return S_OK;
}

void GameObject::Release()
{
}

void GameObject::Update(float TimeDelta)
{
	UpdateAbleCollider(TimeDelta);
}

void GameObject::LateUpdate(float TimeDelta)
{
	CheckCulling();
	// ObjMgr에서 모든 Update 돌린 이후에 필요한 작업 수행할 듯.
}

void GameObject::Render(ID2D1RenderTarget* renderTarget)
{
}

void GameObject::CheckCulling()
{
	//Viewport viewPort = CameraManager::GetInstance()->GetInRect();

	//if (viewPort.left > Pos.x + objectScale.x || viewPort.right < Pos.x - objectScale.x
	//	|| viewPort.top > Pos.y + objectScale.y || viewPort.bottom < Pos.y - objectScale.y)
	//{
	//	bHidden = true;
	//	return;
	//}

	//bHidden = false;
	//if(WINSZE_X)
	// 카메라 컬링 예상
}

void GameObject::UpdateAbleCollider(float TimeDelta)
{
	if (false == bAbleCol)
	{
		collisionCoolTime -= TimeDelta;
		if (0.f >= collisionCoolTime)
		{
			collisionCoolTime = collisionMaxCoolTime;
			bAbleCol = true;
		}
	}
}

GameObject::GameObject() : bActive(true), bDestroy(false), bHidden(false), 
Pos({0.f,0.f}), objectRenderId(RENDER_TILE), interactState(INTERACTSTATE::INTERACT_UNABLE), objectScale({0.f,0.f})
{

}

GameObject::~GameObject()
{

}

void GameObject::Detect(GameObject* obj)
{

}

void GameObject::TakeCollision(float coolTime)
{
	bAbleCol = false;
	collisionCoolTime = coolTime;
}
