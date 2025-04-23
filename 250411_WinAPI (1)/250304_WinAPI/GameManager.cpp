#include "pch.h"
#include "GameManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../MapTool/EditorTile.h"
#include "Tile.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"

using json = nlohmann::json;

void GameManager::Release()
{
	ReleaseInstance();
}

void GameManager::ReleaseStage()
{

}

void GameManager::LoadTile(const char* path, bool isCave)
{
	FILE* fp = fopen(path, "rb");
	if (!fp) return;

	// 1. �ӽ� ���ۿ� ��ü�� ���� �о����
	EditorTile buffer[40 * 32];
	fread(buffer, sizeof(EditorTile), 40 * 32, fp);
	fclose(fp);

	// 2. 2D tileMap�� �ɰ��� ����
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 40; ++x)
		{
			const EditorTile& src = buffer[y * 40 + x];

			// ���ϴ� ���� ����
			Tile* tile = new Tile;

			ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_TILE, tile);

			float renderX = floor((src.pos.x + 0.5f) * GAME_TILE_SIZE);
			float renderY = floor((src.pos.y + 0.5f) * GAME_TILE_SIZE);

			tile->InitTile(src.atlasX, src.atlasY, src.valid, { renderX , renderY }, TileType::GROUND);
			tile->SetCaveRender(isCave);
			
			if (isCave)
			{
				caveTileMap[y][x] = tile;
			}
			else
			{
				tileMap[y][x] = tile;
			}
		}
	}
}

void GameManager::LoadObject(const char* path, bool isCave)
{
	std::ifstream in(path);
	if (!in.is_open())
	{
		printf("Failed to open: %s\n", path);
		return;
	}

	json j;
	in >> j;
	in.close();

	for (const auto& item : j)
	{
		std::string name = item.value("name", "");
		float x = item.value("x", 0.0f);
		float y = item.value("y", 0.0f);
		float w = item.value("width", 0.0f);
		float h = item.value("height", 0.0f);
		bool flipX = item.value("flipX", false);

		// ����
		GameObject* obj = ObjectFactory::Get().Create(name);
		if (!obj)
		{
			int a = 10;
			continue;
		}

		const float tileSize = 128.f; // ���� ������ Ÿ�� ����
		float gx = x / tileSize;
		float gy = y / tileSize;

		float gameTileSize = GAME_TILE_SIZE;
		
		// ���� ��ǥ��� ��ȯ (�ʿ�� �ٽ� �ȼ��� ���ϰų� ���� ��ǥ ���)
		// ���忡 �߰�
		FPOINT worldPos = { gx * gameTileSize, gy * gameTileSize };
		ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_ITEM, obj);
		
		obj->SetPos(worldPos);
		obj->SetFlip(flipX);
		obj->SetCaveRender(isCave);
	}

	printf("Loaded objects from: %s\n", path);
}

void GameManager::GenerateBorderTile()
{
	for (int y = 0; y < 36; ++y)
	{
		for (int x = 0; x < 44; ++x)
		{
			// �߽� ������ ��ŵ (2~17)
			if (x >= 2 && x < 42 && y >= 2 && y < 34)
				continue;

			Tile* tile = new Tile;
			ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_TILE, tile);

			// �� ��ǥ�� �������� ��ȯ: �ܰ��� -2 ~ 17 ����
			int toolX = x - 2;
			int toolY = y - 2;

			float renderX = (toolX + 0.5f) * GAME_TILE_SIZE;
			float renderY = (toolY + 0.5f) * GAME_TILE_SIZE;

			// �ܰ��� atlasX/Y�� -1�� �ְų� �ܰ� ���� Ÿ�� index
			tile->InitTile(0, 2, true, { renderX, renderY }, TileType::BORDER);

			//tileMap[y][x] = tile;
		}
	}
}

void GameManager::GenerateDecoTile()
{
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 40; x++)
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

			if (IsTileValid(x - 1, y, true) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1, true);
				//caveTileMap[y][x]->CreateDecoTile(DecoDirection::LEFT, hasTileAbove);
			}
			
			if (IsTileValid(x + 1, y, true) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1, true);
				//caveTileMap[y][x]->CreateDecoTile(DecoDirection::RIGHT, hasTileAbove);
			}

			if (IsTileValid(x, y - 1, true) == false)
			{
				//caveTileMap[y][x]->CreateDecoTile(DecoDirection::TOP);
			}

			if (IsTileValid(x, y + 1, true) == false)
			{
				//caveTileMap[y][x]->CreateDecoTile(DecoDirection::DOWN);
			}

		}
	}
}

void GameManager::GenerateDecoTile(int tileX, int tileY)
{
	//��
	if (IsTileValid(tileX - 1, tileY) == false)
	{
		bool hasTileAbove = !IsTileValid(tileX, tileY - 1);

		tileMap[tileY][tileX]->CreateDecoTile(DecoDirection::LEFT, hasTileAbove);
	}
	//��
	if (IsTileValid(tileX + 1, tileY) == false)
	{
		bool hasTileAbove = !IsTileValid(tileX, tileY - 1);

		tileMap[tileY][tileX]->CreateDecoTile(DecoDirection::RIGHT, hasTileAbove);
	}
	//��
	if (IsTileValid(tileX, tileY - 1) == false)
	{
		tileMap[tileY][tileX]->CreateDecoTile(DecoDirection::TOP);
	}
	//��
	if (IsTileValid(tileX, tileY + 1) == false)
	{
		tileMap[tileY][tileX]->CreateDecoTile(DecoDirection::DOWN);
	}
}

void GameManager::UpdateAdjacentDecoTiles(int tileX, int tileY)
{
	// �߽� ���� + 4����
	constexpr int offsetX[4] = { -1, 1, 0, 0 };
	constexpr int offsetY[4] = { 0, 0, -1, 1 };

	for (int i = 0; i < 4; ++i)
	{
		int nx = tileX + offsetX[i];
		int ny = tileY + offsetY[i];

		if (nx < 0 || nx >= 40 || ny < 0 || ny >= 32)
			continue;

		if (IsTileValid(nx, ny) == false) continue;
		if (!tileMap[ny][nx]) continue;

		// ���� �ֺ� ���� �˻��ؼ� �ٽ� ����
		GenerateDecoTile(nx, ny);
	}
}

void GameManager::CreateCaveRendertarget(ID2D1RenderTarget* mainRT)
{
	mainRT->CreateCompatibleRenderTarget(
		D2D1::SizeF(WINSIZE_X, WINSIZE_Y),
		&caveRenderTarget
	);
}

void GameManager::Init(const char* path)
{
	string scenePath = "Data/" + string(path);

	string tilePath = scenePath + ".tilemap";
	string objectPath = scenePath + ".json";

	LoadTile(tilePath.c_str());
	//TODO �ӽ�
	LoadObject(objectPath.c_str());
	//GenerateCave(path);

	GenerateDecoTile();
	GenerateBorderTile();
}

void GameManager::GenerateCave(const char* path)
{
	string scenePath = "Data/" + /*string(path)*/ string("caveScene");

	string tilePath = scenePath + ".tilemap";
	string objectPath = scenePath + ".json";


	LoadTile(tilePath.c_str(), true);
	LoadObject(objectPath.c_str(), true);
}

ID2D1BitmapRenderTarget* GameManager::GetCaveRenderTarget()
{
	return caveRenderTarget.Get();
}

void GameManager::DestructionTile(const FPOINT& tilePos)
{
	int tileIndexX, tileIndexY;
	tileIndexX = static_cast<int>(tilePos.x / GAME_TILE_SIZE);
	tileIndexY = static_cast<int>(tilePos.y / GAME_TILE_SIZE);

	if (tileMap[tileIndexY][tileIndexX]->IsValid() == false)
		return;

	tileMap[tileIndexY][tileIndexX]->SetValid(false);
	
	UpdateAdjacentDecoTiles(tileIndexX, tileIndexY);
}

bool GameManager::HasTile(int x, int y)
{
	if (x < 0 || x >= 40 || y < 0 || y >= 32)
		return false;

	return tileMap[y][x]->IsValid();
}

bool GameManager::IsTileValid(int x, int y, bool isCave)
{
	if (x < 0 || x >= 40 || y < 0 || y >= 32)
		return true;

	if (isCave)
	{
		if (caveTileMap[y][x] == nullptr)
			return false;

		return caveTileMap[y][x]->IsValid();
	}
	else
	{
		if (tileMap[y][x] == nullptr)
			return false;

		return tileMap[y][x]->IsValid();
	}
}