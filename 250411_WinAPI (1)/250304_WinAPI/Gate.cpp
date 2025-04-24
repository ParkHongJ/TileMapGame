#include "pch.h"
#include "Gate.h"

HRESULT Gate::Init()
{
	return S_OK;
}

void Gate::Release()
{
}

void Gate::Update(float TimeDelta)
{
}

void Gate::LateUpdate(float TimeDelta)
{
}

void Gate::Render(ID2D1RenderTarget* renderTarget)
{
	//ÃÑ 6¹øÀ» ·»´õÇØ¾ßÇÑ´Ù.
	int dx[] = {-1, 0, 1};
	int dy[] = {-1, 0};


}
