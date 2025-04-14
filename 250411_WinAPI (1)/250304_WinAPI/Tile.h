#pragma once
#include "GameObject.h"

enum TileType
{
	NONE = 0,
	BLOCK
};
struct TileInfo
{
	int atlasX = -1; // Atlas �� Ÿ�� ��ǥ
	int atlasY = -1;
	bool valid = false;
};
class Tile : public GameObject
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

public:
	void InitTile(int atlasX, int atlasY, bool valid, FPOINT pos);
private:
	TileType tileType;
	TileInfo tileInfo;
	class Image* tileImage;
};
