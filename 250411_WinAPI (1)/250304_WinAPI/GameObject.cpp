#include "GameObject.h"

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
	// ObjMgr���� ��� Update ���� ���Ŀ� �ʿ��� �۾� ������ ��.
}

void GameObject::Render(ID2D1HwndRenderTarget* renderTarget)
{
}

void GameObject::CheckCulling()
{
	// ī�޶� �ø� ����
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
Pos({0.f,0.f}), objectRenderId(RENDER_TILE), interactState(INTERACTSTATE::INTERACT_UNABLE)
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
