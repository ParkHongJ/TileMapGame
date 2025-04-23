#pragma once
#include "GameObject.h"

class BoxCollider;
class Image;

enum TileType
{
	NONE = 0,
	GROUND,//��(���ڰ� ���� �� �ִ�)
	BORDER //�ܰ�
};

struct TileInfo
{
	int atlasX = -1; // Atlas �� Ÿ�� ��ǥ
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
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
	void RenderDeco(ID2D1RenderTarget* renderTarget);
	
public:
	void InitTile(int atlasX, int atlasY, bool valid, FPOINT pos, TileType type);
	
	//���ʿ� ���̾��ٸ� �������ϰ�� �ٸ�����Ÿ�Ϸ� ����
	void CreateDecoTile(DecoDirection dir, bool hasTileAbove = false);

	inline bool IsValid() { return tileInfo.valid; }
	void SetValid(bool bValid) { tileInfo.valid = bValid; }

	//Ÿ���� ������
	void Destruction();

private:
	TileType tileType;
	TileInfo tileInfo;
	Image* tileImage;

	BoxCollider* collider;

	vector<DecoInfo*> decos;
	float tileScale;
};
