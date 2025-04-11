#pragma once
#include "GameObject.h"
#include <vector>

class Enemy;
class EnemyManager : public GameObject
{
private: 
	Enemy* enemys;
	vector<Enemy*> vecEnemys;
	wchar_t szText[128];

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void AddEnemy(int size);
};

