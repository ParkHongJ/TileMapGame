#pragma once
#include "GameObject.h"

// ���� Ÿ�� ����
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILE_SIZE	32

// ���� �׸��� ���� ����
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
class TilemapTool
{
private:
	Image* sampleTile;
	RECT rcSampleTile;

	TILE_INFO tileInfo[TILE_X * TILE_Y];
	RECT rcMain;

	POINT selectedTile;

	Button* saveButton;

public:
	HRESULT Init();
	void Release();
	void Update(float TimeDelta);
	void Render(HDC hdc);

	void Save();
	void Load();

	TilemapTool() {};
	virtual ~TilemapTool() {};
};

