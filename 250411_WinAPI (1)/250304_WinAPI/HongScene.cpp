#include "HongScene.h"
#include "ObjectManager.h"
#include "Tile.h"
#include "../MapTool/EditorTile.h"
#include "CommonFunction.h"
#include "CollisionManager.h"
#include "DummyHongPlayer.h"
#include "Image.h"
void HongScene::LoadTile(const char* path)
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
			tile->Init();

			float renderX = (src.pos.x + 0.5f) * GAME_TILE_SIZE;
			float renderY = (src.pos.y + 0.5f) * GAME_TILE_SIZE;

			tile->InitTile(src.atlasX, src.atlasY, src.valid, { renderX , renderY });

			tileMap[y][x] = tile;
			//tileMap[y][x].atlasX = src.atlasX;
			//tileMap[y][x].atlasY = src.atlasY;
			//tileMap[y][x].valid = src.valid;

			// posX, posY�� �ڵ����� ����
			//tileMap[y][x].posX = x;
			//tileMap[y][x].posY = y;
		}
	}
}

HRESULT HongScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	CollisionManager::GetInstance()->Init();
	ObjectManager::GetInstance()->Init();
	ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);

	ImageManager::GetInstance()->AddImage("CaveDecoDown", L"Textures/CaveDecoDown.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoTop", L"Textures/CaveDecoTop.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoRight", L"Textures/CaveDecoRight.png", renderTarget);

	backGround = ImageManager::GetInstance()->AddImage("Test123123", L"Textures/char_black.png", 16, 16, renderTarget);

	LoadTile("Data/map1.tilemap");

	GenerateDecoTile();

	player = new DummyHongPlayer;
	return S_OK;
}

void HongScene::Release()
{
	CollisionManager::GetInstance()->Release();
	ObjectManager::GetInstance()->Release();

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (tileMap[y][x])
			{
				tileMap[y][x]->Release();
				delete tileMap[y][x];
			}
		}
	}
	player->Release();
	delete player;
}

void HongScene::Update(float TimeDelta)
{
	player->Update(TimeDelta);
	ObjectManager::GetInstance()->Update(TimeDelta);
	CollisionManager::GetInstance()->Update(TimeDelta);

	float frameDuration = 0.125f; // �� �������� �����Ǵ� �ð�
	FrameTime += TimeDelta;

	if (FrameTime >= frameDuration)
	{
		FrameTime = 0.f;
		currentFrame++; // ���� ����������
	}

	if (currentFrame > 16)
	{
		currentFrame = 0;
	}
}

void HongScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	player->Render(renderTarget);
	ObjectManager::GetInstance()->Render(renderTarget);

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (tileMap[y][x])
			{
				tileMap[y][x]->Render(renderTarget);
			}
		}
	}
	
	backGround->FrameRender(renderTarget, 100, 100, currentFrame, 0);

#ifdef _DEBUG
	CollisionManager::GetInstance()->DebugRender(renderTarget);
#endif _DEBUG
}

void HongScene::GenerateDecoTile()
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

bool HongScene::IsTileValid(int x, int y)
{
	if (x < 0 || x >= 16 || y < 0 || y >= 16)
		return false;
	
	if (tileMap[y][x] == nullptr)
		return false;

	return tileMap[y][x]->IsValid();
}
