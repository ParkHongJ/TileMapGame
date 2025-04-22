#include "pch.h"
#include "TestMonster.h"
#include "TestAnimationObject.h"

TestMonster::TestMonster()
{
}

TestMonster::~TestMonster()
{
}

HRESULT TestMonster::Init()
{
	return S_OK;
}

void TestMonster::Release()
{
}

void TestMonster::Update(float TimeDelta)
{
}

void TestMonster::LateUpdate(float TimeDelta)
{
}

void TestMonster::Render(ID2D1RenderTarget* renderTarget)
{

}

void TestMonster::Detect(GameObject* obj)
{
	if (auto player = obj->GetType<TestAnimationObject>())
	{

		//SetDestroy();
	}
}
