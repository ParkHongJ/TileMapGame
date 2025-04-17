#pragma once
#include "Singleton.h"

class Tile;
//게임 매니저는 각씬마다 있어야할까?

//일단 각씬마다 있는형태.
class GameManager : public Singleton<GameManager>
{
public:
	void Release();


	void LoadTile(const char* path);
	void LoadObject(const char* path);

	void GenerateDecoTile();

	void Init(const char* path);
private:
	bool IsTileValid(int x, int y);

private:
	//나중에 게임매니저로 빼든해야한다.
	Tile* tileMap[16][16];
};

