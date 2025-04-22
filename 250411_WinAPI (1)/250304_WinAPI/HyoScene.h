#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "HyoCharacter.h"
#include "TestHyoBackGround.h"

class Image;
//class Camera;
class HyoCharacter;
class TestHyoBackGround;
class HyoScene : public Scene
{
public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	void TileRender(HDC hdc);



	HyoScene() {};
	virtual ~HyoScene() {};

private:
	Image* background;
	TestHyoBackGround testTile[TILE_COUNT][TILE_COUNT];
	HyoCharacter* testBackGround;
	HyoCharacter* testCamera;
	// Camera* camera;

	FPOINT cameraPos;
	float mapSizeHeight;
	float maptSizeWidth;
	bool canRender;
};

