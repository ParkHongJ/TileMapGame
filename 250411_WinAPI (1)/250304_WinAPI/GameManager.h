#pragma once
#include "Singleton.h"

class Tile;
//���� �Ŵ����� �������� �־���ұ�?

//�ϴ� �������� �ִ�����.
class GameManager : public Singleton<GameManager>
{
public:
	void Release();
	void ReleaseStage();

	void LoadTile(const char* path, bool isCave = false);
	void LoadObject(const char* path, bool isCave = false);
	void GenerateBorderTile();
	void GenerateDecoTile();

	void CreateCaveRendertarget(ID2D1RenderTarget* mainRT);
	void Init(const char* path);
	void GenerateCave(const char* path);
	ID2D1BitmapRenderTarget* GetCaveRenderTarget();
private:
	bool IsTileValid(int x, int y);

private:
	Tile* tileMap[36][44];
	ComPtr<ID2D1BitmapRenderTarget> caveRenderTarget;
};

