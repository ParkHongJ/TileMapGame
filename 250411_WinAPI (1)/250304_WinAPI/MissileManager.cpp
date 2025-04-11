#include "MissileManager.h"
#include "Missile.h"

HRESULT MissileManager::Init()
{
	vecMissiles.resize(30);
	for (iterMissiles = vecMissiles.begin();
		iterMissiles != vecMissiles.end(); iterMissiles++)
	{
		(*iterMissiles) = new Missile();
		(*iterMissiles)->Init();
	}

	return S_OK;
}

void MissileManager::Release()
{
	for (iterMissiles = vecMissiles.begin();
		iterMissiles != vecMissiles.end(); iterMissiles++)
	{
		if (*iterMissiles)
		{
			(*iterMissiles)->Release();
			delete (*iterMissiles);
			(*iterMissiles) = nullptr;
		}
	}
	vecMissiles.clear();
}

void MissileManager::Update()
{
}

void MissileManager::Render(HDC hdc)
{
}
