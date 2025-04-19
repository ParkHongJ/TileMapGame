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

	void LoadTile(const char* path);
	void LoadObject(const char* path);
	void GenerateBorderTile();
	void GenerateDecoTile();

	void Init(const char* path);
private:
	bool IsTileValid(int x, int y);

private:
	//���߿� ���ӸŴ����� �����ؾ��Ѵ�.
	Tile* tileMap[36][44];
};

