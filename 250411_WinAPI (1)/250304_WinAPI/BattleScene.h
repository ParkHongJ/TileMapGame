#pragma once
#include "GameObject.h"

class Image;
class EnemyManager;
class BattleScene : public GameObject
{
private:
	Image* backGround;
	EnemyManager* enemyManager;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	BattleScene() {};
	virtual ~BattleScene() {};
};

