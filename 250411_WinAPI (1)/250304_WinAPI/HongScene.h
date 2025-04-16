#pragma once
#include "Scene.h"
class Tile;
class DummyHongPlayer;
class HongScene : public Scene
{
private:
	Image* backGround;
	DummyHongPlayer* player;

	//���߿� ���ӸŴ����� �����ؾ��Ѵ�.
	Tile* tileMap[16][16];

	void LoadTile(const char* path);
public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

private:
	void GenerateDecoTile();
	bool IsTileValid(int x, int y);
public:
	HongScene() {};
	virtual ~HongScene() {};
};
