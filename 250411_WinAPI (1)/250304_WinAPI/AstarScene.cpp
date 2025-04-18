#include "pch.h"
#include "AstarScene.h"
#include <algorithm>

HRESULT AstarTile::Init()
{
	return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
	this->idX = idX;
	this->idY = idY;

	center.x = idX * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);
	center.y = idY * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);

	rc.left = idX * ASTAR_TILE_SIZE;
	rc.right = rc.left + ASTAR_TILE_SIZE;
	rc.top = idY * ASTAR_TILE_SIZE;
	rc.bottom = rc.top + ASTAR_TILE_SIZE;

	costFromStart = 0.0f;
	costToGoal = 0.0f;
	totalCost = 0.0f;

	type = AstarTileType::None;

	parentTile = nullptr;

	color = RGB(100, 100, 100);
	hBrush = CreateSolidBrush(color);

	return S_OK;
}

void AstarTile::Release()
{
	DeleteObject(hBrush);
}

void AstarTile::Update()
{
}

void AstarTile::Render(ID2D1HwndRenderTarget* renderTarget)
{
	/*hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	FillRect(hdc, &rc, hBrush);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

	SelectObject(hdc, hOldBrush);*/
}

void AstarTile::SetColor(COLORREF color)
{
	this->color = color;
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(color);
}

HRESULT AstarScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// ���ιݺ� (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// ���ιݺ� (x)
		{
			map[i][j].Init(j, i);
		}
	}

	startTile = &(map[3][3]);
	startTile->SetColor(RGB(255, 0, 0));
	startTile->SetType(AstarTileType::Start);

	currTile = startTile;

	destTile = &(map[10][19]);
	destTile->SetColor(RGB(0, 0, 255));
	destTile->SetType(AstarTileType::End);

	return S_OK;
}

void AstarScene::Release()
{
}

void AstarScene::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
	{
		// g_ptMouse�� �ε����� ���
		int x, y;
		x = g_ptMouse.x / ASTAR_TILE_SIZE;
		y = g_ptMouse.y / ASTAR_TILE_SIZE;

		if (0 <= x && x < ASTAR_TILE_COUNT &&
			0 <= y && y < ASTAR_TILE_COUNT)
		{
			// �����̳� �������� �ƴ� ��
			if (map[y][x].GetType() != AstarTileType::Start &&
				map[y][x].GetType() != AstarTileType::End)
			{
				map[y][x].SetColor(RGB(100, 150, 100));
				map[y][x].SetType(AstarTileType::Wall);
			}
		}
	}


	// TODO 
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		FindPath();
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("������_1");
	}
}

void AstarScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// ���ιݺ� (y)
	//{
	//	for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// ���ιݺ� (x)
	//	{
	//		map[i][j].Render(hdc);
	//	}
	//}
}

void AstarScene::FindPath()
{
	if (currTile)
	{
		// ������ �ִ� �̵������� Ÿ�ϵ��� F�� ��� �ĺ��� �ִ´�.
		AddOpenList(currTile);

		// �ĺ��� �� F���� ���� ���� Ÿ���� ���� currTile ����
		currTile = GetLowestCostTile();
		closeList.push_back(currTile);

		// �������� �Ǵ�
		if (currTile == destTile)
		{
			PrintPath();
			return;
		}

		// �ݺ�
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
		{
			FindPath();
		}
	}
}

void AstarScene::AddOpenList(AstarTile* currTile)
{
	static const int dx[4] = { -1, 1, 0, 0 };
	static const int dy[4] = { 0, 0, -1, 1 };

	for (int i = 0; i < 4; i++)
	{
		int nx = currTile->idX + dx[i];
		int ny = currTile->idY + dy[i];

		if (nx < 0 || nx >= ASTAR_TILE_COUNT ||
			ny < 0 || ny >= ASTAR_TILE_COUNT)
			continue;

		AstarTile* neighbor = &map[ny][nx];
		if (!IsValidNeighbor(neighbor)) continue;

		// �̵� ��� Ȯ�� �� ���� ���� Ȯ��
		UpdateNeighborCosts(neighbor, currTile);
	}
}

void AstarScene::UpdateNeighborCosts(AstarTile* neighbor, AstarTile* current)
{
	float newG = current->costFromStart + 1.0f;
	bool inOpen 
		= find(openList.begin(), openList.end(), neighbor) != openList.end();

	if (inOpen == false || newG < neighbor->costFromStart)
	{
		neighbor->costFromStart = newG;
		neighbor->costToGoal = Heuristic(neighbor, destTile);
		neighbor->totalCost = neighbor->costFromStart + neighbor->costToGoal;
		neighbor->parentTile = current;

		if (!inOpen)
		{
			neighbor->SetColor(RGB(255, 0, 255));
			openList.push_back(neighbor);
		}
	}
}

float AstarScene::Heuristic(AstarTile* a, AstarTile* b)
{
	int dx = abs(a->idX - b->idX);
	int dy = abs(a->idY - b->idY);
	return dx + dy;
}

AstarTile* AstarScene::GetLowestCostTile()
{
	auto iter = min_element(openList.begin(), openList.end(), [](AstarTile* a, AstarTile* b) {
		return a->totalCost < b->totalCost;
		});
	AstarTile* tile = *iter;
	openList.erase(iter);

	return tile;
}

void AstarScene::PrintPath()
{
	AstarTile* curr = destTile;
	while (curr != nullptr && curr != startTile)
	{
		curr->SetColor(RGB(0, 0, 0));
		curr = curr->parentTile;
	}
}

bool AstarScene::IsValidNeighbor(AstarTile* neighbor)
{
	bool isNotClose
		= find(closeList.begin(), closeList.end(), neighbor) == closeList.end();

	return neighbor->GetType() != AstarTileType::Wall && isNotClose;
}
