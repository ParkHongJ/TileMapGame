#pragma once
#include "Scene.h"

class Image;
class EnemyManager;
class BattleScene : public Scene
{
private:
	Image* backGround;

public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	BattleScene() {};
	virtual ~BattleScene() {};
};

