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
}

void GameObject::LateUpdate(float TimeDelta)
{
	// ObjMgr에서 모든 Update 돌린 이후에 필요한 작업 수행할 듯.
	// 아마 자체 판단한 카메라 컬링?
}

void GameObject::Render(ID2D1HwndRenderTarget* renderTarget)
{
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
