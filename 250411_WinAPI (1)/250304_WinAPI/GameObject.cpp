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
	// ObjMgr���� ��� Update ���� ���Ŀ� �ʿ��� �۾� ������ ��.
	// �Ƹ� ��ü �Ǵ��� ī�޶� �ø�?
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
