#pragma once
#include "GameObject.h"

#define TILE_SIZE	30
#define TILE_COUNT	90

class Image;
class TestHyoBackGround : public GameObject
{
private:
	float idX, idY;
	FPOINT center;
	RECT rc;

	Image* image;

public:
	HRESULT Init(int idX, int idY);
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	RECT GetRect() { return rc; }

	TestHyoBackGround() {}
	virtual ~TestHyoBackGround();
};

//typedef struct tagTile	// ���ܿ� �׷��� �ϳ��ϳ��� �̸�
//{
//	RECT rc;
//	int frameX;
//	int frameY;
//} TILE_INFO;
//
//
//class Image;
//class TestHyoBackGround : public GameObject
//{
//public:
//	TestHyoBackGround();
//	virtual ~TestHyoBackGround();
//
//	virtual HRESULT Init() override;
//	virtual void Update(float TimeDelta) override;
//	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
//	virtual void Release() override;
//
//private:
//	Image* image;
//
//	RECT rcSampleTile; // ����Ÿ�Կ������� 
//
//	TILE_INFO tileInfo[TILE_X * TILE_Y];
//	RECT rcMain; // ���ο������� 
//
//	POINT selectedTile;
//
//	//vector <Tile> tiles;
//	int tileSize;
//	int startX;
//	int endX;
//	int startY;
//	int endY;
//};

