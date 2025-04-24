#include "pch.h"
#include "GameManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../MapTool/EditorTile.h"
#include "Tile.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"

using json = nlohmann::json;

std::vector<JumpNode*> GameManager::jumpNodes;

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
		

		if (name == "Character")
			obj->SetPlayerKey(playerImageKey);

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
	//default key
	string scenePath = "Data/" + string(path);

	string tilePath = scenePath + ".tilemap";
	string objectPath = scenePath + ".json";

	LoadTile(tilePath.c_str());
	//TODO �ӽ�
	LoadObject(objectPath.c_str());
	//GenerateCave(path);

	GenerateDecoTile();
	GenerateBorderTile();
	BuildJumpNodesFromTileMap();
	LinkWalkableNeighbors();
	LinkJumpableNeighbors();
	LinkFallNeighbors();
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

void GameManager::SetPlayerImageKey(string key)
{
	playerImageKey = key;
}

vector<JumpNode*> GameManager::FindPath(JumpNode* start, JumpNode* goal)
{
	std::vector<JumpNode*> openSet;
	std::vector<JumpNode*> result;

	for (JumpNode* node : jumpNodes)
	{
		node->gCost = FLT_MAX;
		node->hCost = 0.f;
		node->parent = nullptr;
		node->visited = false;
	}

	start->gCost = 0.f;
	start->hCost = Heuristic(start->worldPos, goal->worldPos);
	openSet.push_back(start);

	while (!openSet.empty())
	{
		// 1. ���� fCost�� ���� ��� ������
		auto compare = [](JumpNode* a, JumpNode* b)
			{
				return a->fCost() > b->fCost();
			};
		std::sort(openSet.begin(), openSet.end(), compare);

		JumpNode* current = openSet.front();
		openSet.erase(openSet.begin());
		current->visited = true;

		// 2. �����ϸ� ��� ����
		if (current == goal)
		{
			JumpNode* trace = goal;
			while (trace)
			{
				result.push_back(trace);
				trace = trace->parent;
			}
			std::reverse(result.begin(), result.end());
			return result;
		}

		// 3. �̿� ��� �˻�
		for (JumpNode* neighbor : current->neighbors)
		{
			if (neighbor->visited)
				continue;

			float tentativeG = current->gCost + Heuristic(current->worldPos, neighbor->worldPos);

			if (tentativeG < neighbor->gCost)
			{
				neighbor->gCost = tentativeG;
				neighbor->hCost = Heuristic(neighbor->worldPos, goal->worldPos);
				neighbor->parent = current;

				if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
					openSet.push_back(neighbor);
			}
		}
	}

	return result; // ��� ����
}

JumpNode* GameManager::FindClosestJumpNode(const FPOINT& worldPos)
{
	JumpNode* closest = nullptr;
	float closestDist = FLT_MAX;

	for (JumpNode* node : jumpNodes)
	{
		float dx = worldPos.x - node->worldPos.x;
		float dy = worldPos.y - node->worldPos.y;
		float dist = dx * dx + dy * dy;

		if (dist < closestDist)
		{
			closestDist = dist;
			closest = node;
		}
	}

	return closest;
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

void GameManager::BuildJumpNodesFromTileMap()
{
	jumpNodes.clear();

	for (int y = 1; y < 32; ++y)
	{
		for (int x = 0; x < 40; ++x)
		{
			Tile* tile = tileMap[y][x];
			Tile* tileAbove = tileMap[y - 1][x];

			// �ٴ��̰�, �� ���� ��������� ������� ����
			if (tile && tile->IsValid() && !tileAbove->IsValid())
			{
				JumpNode* node = new JumpNode;
				node->x = x; // ������ ĳ���Ͱ� ���� ��ġ�� ��
				node->y = y - 1; // ������ ĳ���Ͱ� ���� ��ġ�� ��
				node->worldPos = {
					x * GAME_TILE_SIZE + GAME_TILE_SIZE * 0.5f,
					(y - 1) * GAME_TILE_SIZE + GAME_TILE_SIZE * 0.5f
				};

				jumpNodes.push_back(node);
			}
		}
	}

}

void GameManager::LinkWalkableNeighbors()
{
	for (auto* from : GameManager::jumpNodes)
	{
		for (auto* to : GameManager::jumpNodes)
		{
			if (from == to)
				continue;

			// ���� ����(��)�� �ִ� ��츸 üũ
			if (from->y != to->y)
				continue;

			int dx = to->x - from->x;

			if (abs(dx) == 1) // ��/�쿡 �پ� �ִ� ���
			{
				// �߰��� ���� �ִ��� �˻� (����)
				if (IsWalkableBetween(from->x, from->y, to->x, to->y))
				{
					from->neighbors.push_back(to);
					// ���⼺ ����. ��������� �� �Ÿ�:
					// to->neighbors.push_back(from);
				}
			}
		}
	}

	printf("�ȱ� ���� �Ϸ�\n");
}

bool GameManager::IsWalkableBetween(int x1, int y1, int x2, int y2)
{
	int dir = x2 - x1; // -1(����) or +1(������)
	if (abs(dir) != 1) return false;

	int midX = x1 + dir;
	int y = y1;

	if (midX < 0 || y < 0 || midX >= 40 || y >= 32)
		return false;
	if (midX < 0 || y - 1 < 0 || midX >= 40 || y - 1 >= 32)
		return false;

	return true;
}

void GameManager::LinkFallNeighbors()
{
	constexpr int MaxFallRange = 10;

	for (JumpNode* from : jumpNodes)
	{
		int tx = from->x;
		int ty = from->y;

		// ��, �� �� ĭ �̵� �� �Ʒ��� ���ϸ� ���
		for (int dx = -1; dx <= 1; dx += 2) // -1, +1
		{
			int fallX = tx + dx;
			if (fallX < 0 || fallX >= 40)
				continue;

			for (int dy = 1; dy <= MaxFallRange; ++dy)
			{
				int fallY = ty + dy;
				if (fallY >= 32)
					break;

				// �߰��� �� ������ �Ұ�
				if (tileMap[fallY][fallX]->IsValid())
					break;

				// ���� ����: �� Ÿ�� + �ٷ� �Ʒ� solid
				if (IsStandableTile(fallX, fallY))
				{
					int sideX = fallX;
					int sideY = from->y;

					int downX = from->x;
					int downY = fallY;

					if (tileMap[sideY][sideX]->IsValid() && tileMap[downY][downX]->IsValid())
					{
						// �ڳʿ� ���� �߻� �� ���� ����
						continue;
					}

					JumpNode* to = FindJumpNodeAt(fallX, fallY);
					if (to)
					{
						from->neighbors.push_back(to);
						printf("FALL: (%d,%d) �� (%d,%d)\n", tx, ty, fallX, fallY);
					}
					break;
				}
			}
		}
	}
}

void GameManager::LinkJumpableNeighbors()
{
	constexpr float MaxJumpHeight = 48.f;  // �ִ� ���� ���� (��: 2.5Ÿ��)
	constexpr float MaxJumpDistance = 96.f; // �ִ� ���� ���� �Ÿ� (��: 4Ÿ��)

	for (JumpNode* from : jumpNodes)
	{
		for (JumpNode* to : jumpNodes)
		{
			if (from == to)
				continue;

			FPOINT diff = to->worldPos - from->worldPos;

			// ���⼺ �ִ� ���� (to�� ��/�����ʿ� ����)
			if (diff.y > 0.f) continue; // �Ʒ��δ� ���ϴϱ� ����

			if (fabs(diff.x) > MaxJumpDistance) continue;
			if (fabs(diff.y) > MaxJumpHeight) continue;

			if (IsJumpPathClear(from->worldPos, to->worldPos))
			{
				from->neighbors.push_back(to); // �ܹ��� ����
			}
		}
	}
}

bool GameManager::IsJumpPathClear(FPOINT from, FPOINT to)
{
	constexpr int steps = 8;

	for (int i = 1; i < steps; ++i)
	{
		float t = static_cast<float>(i) / steps;

		float x = from.x + (to.x - from.x) * t;
		float y = from.y + (to.y - from.y) * t;

		IPOINT current = WorldToTile(x, y);

		if (tileMap[current.y][current.x]->IsValid())
			return false;

		// �밢�� ���� ����
		// �ڳ� Ÿ�� 2��: ����, ���� ���� �̵��� ���
		float stepX = from.x + (to.x - from.x) * t;
		float stepY = from.y + (to.y - from.y) * t;

		IPOINT xOnly = WorldToTile(stepX, from.y); // ���� �̵��� ���
		IPOINT yOnly = WorldToTile(from.x, stepY); // ������ �̵��� ���

		if (tileMap[xOnly.y][xOnly.x]->IsValid() && tileMap[yOnly.y][yOnly.x]->IsValid())
		{
			return false; // �ڳ� ���� �߻�
		}
	}

	return true;
}

JumpNode* GameManager::FindJumpNodeAt(int x, int y)
{
	for (JumpNode* node : jumpNodes)
	{
		if (node->x == x && node->y == y)
			return node;
	}
	return nullptr;
}

IPOINT GameManager::WorldToTile(float x, float y)
{
	int tileIndexX, tileIndexY;
	tileIndexX = static_cast<int>(x / GAME_TILE_SIZE);
	tileIndexY = static_cast<int>(y / GAME_TILE_SIZE);

	return { tileIndexX , tileIndexY };
}

bool GameManager::IsStandableTile(int x, int y)
{
	if (y + 1 >= 32)
		return false;

	// ���� ������ ��ġ�� ��� �־�� ��
	if (tileMap[y][x]->IsValid()) // �� �浹�̸� false
		return false;

	// �� ���� solid���� ��
	if (!tileMap[y + 1][x]->IsValid())
		return false;

	return true;
}
