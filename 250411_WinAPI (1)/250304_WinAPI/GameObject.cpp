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

GameObject::GameObject() : bActive(true), bDestroy(false), bHidden(false)
{

}

GameObject::~GameObject()
{

}
