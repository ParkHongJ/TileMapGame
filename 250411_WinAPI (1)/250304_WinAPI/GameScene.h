#pragma once
#include "Scene.h"
class GameScene : public Scene
{
private:
	class Image* background;
public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void LateUpdate(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

private:
	void RenderBackground(ID2D1RenderTarget* rt);
public:
	GameScene() {};
	virtual ~GameScene() {};
};

