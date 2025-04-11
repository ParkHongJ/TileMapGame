#pragma once
#include "GameObject.h"

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILE_SIZE	32

// 메인 그리기 공간 정보
#define TILE_X	20
#define TILE_Y	20

typedef struct tagTile
{
	RECT rc;
	int frameX;
	int frameY;
} TILE_INFO;

class Image;
class Button;
class TilemapTool : public GameObject
{
private:
	Image* sampleTile;
	RECT rcSampleTile;

	TILE_INFO tileInfo[TILE_X * TILE_Y];
	RECT rcMain;

	POINT selectedTile;

	Button* saveButton;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Save();
	void Load();

	TilemapTool() {};
	virtual ~TilemapTool() {};
};

