#include "EnemyManager.h"
#include "Enemy.h"

// 1. WinSizeX, Y를 사막배경에 맞춰서 변경
// 2. 백그라운드를 사막으로 변경
// 3. 플레이어 (rocket.bmp) 구현
// 4. 적 10마리 좌우 움직임 구현
// 5. 미사일 매니저 구현 - 플레이어도 사용, 적 사용
// 6. 적이 1마리씩 밑으로 돌격한다. (중간에 미사일 1발 발사)

HRESULT EnemyManager::Init()
{
	// 1. 배열
	//enemys = new Enemy[10];
	//for (int i = 0; i < 10; i++)
	//{
	//	enemys[i].Init();
	//}

	// 2. vector
	/*
		push_back() 함수의 단점 : 
	*/
	//for (int i = 0; i < 10; i++)
	//{
	//	vecEnemys.push_back(new Enemy);
	//	vecEnemys[i]->Init();
	//}

	vecEnemys.resize(10);
	for (int i = 0; i < 10; i++)
	{
		vecEnemys[i] = new Enemy();
		vecEnemys[i]->Init(310.0f + 130.0f * (i % 5) , 
			80.0f + 90.0f * (i / 5));
	}

	return S_OK;
}

void EnemyManager::Release()
{
	for (int i = 0; i < 10; i++)
	{
		vecEnemys[i]->Release();
		delete vecEnemys[i];
	}
	vecEnemys.clear();	// 원소가 모두 제거되고 capacity는 유지된다.
}

void EnemyManager::Update(float TimeDelta)
{
	for (int i = 0; i < vecEnemys.size(); i++)
	{
		vecEnemys[i]->Update();
	}
}

void EnemyManager::Render(HDC hdc)
{
	for (int i = 0; i < vecEnemys.size(); i++)
	{
		vecEnemys[i]->Render(hdc);
	}
}

void EnemyManager::AddEnemy(int size)
{
	for (int i = 0; i < size; i++)
	{
		vecEnemys.push_back(new Enemy());
		vecEnemys.back()->Init();
	}
}
