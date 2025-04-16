#include "GameManager.h"

#include "../MapTool/EditorTile.h"
#include "Tile.h"

void GameManager::Release()
{
	ReleaseInstance();
}

void GameManager::LoadTile(const char* path)
{
	FILE* fp = fopen(path, "rb");
	if (!fp) return;

	// 1. �ӽ� ���ۿ� ��ü�� ���� �о����
	EditorTile buffer[16 * 16];
	fread(buffer, sizeof(EditorTile), 16 * 16, fp);
	fclose(fp);

	// 2. 2D tileMap�� �ɰ��� ����
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			const EditorTile& src = buffer[y * 16 + x];

			// ���ϴ� ���� ����
			Tile* tile = new Tile;

			ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_TILE, tile);

			float renderX = (src.pos.x + 0.5f) * GAME_TILE_SIZE;
			float renderY = (src.pos.y + 0.5f) * GAME_TILE_SIZE;

			tile->InitTile(src.atlasX, src.atlasY, src.valid, { renderX , renderY });

			tileMap[y][x] = tile;
		}
	}
}

void GameManager::GenerateDecoTile()
{
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{

			//�� �� �� �쿡 ������ Ÿ���� ���ٸ�.
			//����Ÿ�� �߰�

			//������ ���Ѿ��Ѵ�. �� �� ����Ÿ���� ���� ����Ÿ�Ͽ� ���� �������� �ϴϱ�.
			//�� �ܰ��� ���� ó���� �����Ѵ�.

			//��
			if (IsTileValid(x - 1, y) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1);
				tileMap[y][x]->CreateDecoTile(DecoDirection::LEFT, hasTileAbove);
			}

			//��
			if (IsTileValid(x + 1, y) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1);
				tileMap[y][x]->CreateDecoTile(DecoDirection::RIGHT, hasTileAbove);
			}

			//��
			if (IsTileValid(x, y - 1) == false)
			{
				tileMap[y][x]->CreateDecoTile(DecoDirection::TOP);
			}

			//��
			if (IsTileValid(x, y + 1) == false)
			{
				tileMap[y][x]->CreateDecoTile(DecoDirection::DOWN);
			}

		}
	}
}

void GameManager::Init(const char* path)
{
	LoadTile(path);
	GenerateDecoTile();
}

bool GameManager::IsTileValid(int x, int y)
{
	if (x < 0 || x >= 16 || y < 0 || y >= 16)
		return false;

	if (tileMap[y][x] == nullptr)
		return false;

	return tileMap[y][x]->IsValid();
}