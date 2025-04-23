#pragma once
#include "GameObject.h"

class BoxCollider;
class Image;

enum TileType
{
	NONE = 0,
	GROUND,//땅(데코가 붙을 수 있는)
	BORDER //외곽
};

struct TileInfo
{
	int atlasX = -1; // Atlas 내 타일 좌표
	int atlasY = -1;
	bool valid = false;
};

enum class DecoDirection
{
	NONE = 0,
	TOP,
	DOWN,
	LEFT,
	RIGHT
};

struct DecoInfo
{
	DecoDirection dir;
	Image* decoImage;

	int atlasX = 0;
	int atlasY = 0;
};

class Tile : public GameObject
{
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void LateUpdate(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)
	void RenderDeco(ID2D1RenderTarget* renderTarget);
	
public:
	void InitTile(int atlasX, int atlasY, bool valid, FPOINT pos, TileType type);
	
	//위쪽에 블럭이없다면 오른쪽일경우 다른데코타일로 변경
	void CreateDecoTile(DecoDirection dir, bool hasTileAbove = false);

	inline bool IsValid() { return tileInfo.valid; }
	void SetValid(bool bValid) { tileInfo.valid = bValid; }

	//타일이 터져요
	void Destruction();

private:
	TileType tileType;
	TileInfo tileInfo;
	Image* tileImage;

	BoxCollider* collider;

	vector<DecoInfo*> decos;
	float tileScale;
};
