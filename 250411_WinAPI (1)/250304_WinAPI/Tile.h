#pragma once
#include "GameObject.h"

enum TileType
{
	NONE = 0,
	BLOCK
};
struct TileInfo
{
	int atlasX = -1; // Atlas 내 타일 좌표
	int atlasY = -1;
	bool valid = false;
};
class Tile : public GameObject
{
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)

public:
	void InitTile(int atlasX, int atlasY, bool valid, FPOINT pos);
private:
	TileType tileType;
	TileInfo tileInfo;
	class Image* tileImage;
};
