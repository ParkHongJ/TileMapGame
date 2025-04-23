#include "pch.h"
#include "RopeController.h"
#include "Rope.h"


HRESULT RopeController::Init()
{
	// ¥Û¥Û¿Ã 12, 9, ~ 15, 9 / 0, 12 ~ 9, 12
	for (int i = 0; i < 4; ++i)
	{
		Rope* temp = new Rope();
		temp->SetFrame(i + 12, 9);
		ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
		FPOINT dest = { 300, float(i * 30) };
		temp->SetPos(dest);
	}

	for (int i = 0; i < 10; ++i)
	{
		Rope* temp = new Rope();
		temp->SetFrame(i, 12);
		ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
		FPOINT dest = { 300, float((i * 30) + 120) };
		temp->SetPos(dest);
	}

	return S_OK;
}

void RopeController::Update(float TimeDelta)
{
}

void RopeController::Render(ID2D1RenderTarget* renderTarget)
{
}

void RopeController::Release()
{

}

void RopeController::Detect(GameObject* obj)
{

}
