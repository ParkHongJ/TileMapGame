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
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	BattleScene() {};
	virtual ~BattleScene() {};
};

