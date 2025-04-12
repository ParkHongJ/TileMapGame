#pragma once
#include "Scene.h"

class Image;
class EnemyManager;
class BattleScene : public Scene
{
private:
	Image* backGround;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	BattleScene() {};
	virtual ~BattleScene() {};
};

