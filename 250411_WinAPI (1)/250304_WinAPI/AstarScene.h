#pragma once
#include "GameObject.h"
#include "config.h"

/*
	열린 목록 : 탐색할 노드들을 저장, 시작 노드 추가
	닫힌 목록 : 이미 탐색한 노드들을 저장, 초기에는 비어 있음
	g(n) : 시작 노드에서 노드 n까지 이동하는 실제 비용 
	h(n) : 노드 n에서 목표 노드까지의 추정비용(휴리스틱)
	f(n) : 노드 n에서 목표 노드까 도달하는 총 비용, f(n) = g(n) + h(n)

	탐색 과정 :
	 1) 열린 목록에서 f값이 가장 작은 노드 n을 선택해서 현재 노드로 설정
	 2) 현재 노드 n이 목표노드라면 경로 탐색을 종료하고, 경로를 반환
	 3) 그렇지 않으면, 현재 노드를 열린 목록에서 제거하고 닫힌 목록에 추가
	 4) 현재 노드 n에 대한 모든 이웃노드 m에 대해서 다음 단계를 수행
		4_1) m이 닫힌 목록에 있으면 무시
		4_2) m이 열린 목록에 없으면 추가( g(m), h(m), f(m) / 
			부모 노드(이전 노드)를 n으로 설정)
		4_3) m이 이미 열린 목록에 있고, 현재 경로를 통해서 더 작은 g값을
			가질 수 있다면, g(m), f(m)갱신 , 부모 노드도 갱신

	경로 구성 :
	목표 노드에서 시작 노드까지 부모노드를 경로를 역추적해서 경로 구성
*/

#define ASTAR_TILE_SIZE		30
#define ASTAR_TILE_COUNT	20

enum class AstarTileType { Start, End, Wall, None };

class AstarTile : public GameObject
{
private:
	int idX, idY;
	POINT center;
	RECT rc;
	AstarTileType type;

	float costFromStart;	// g : 시작점부터 현재 노드까지의 비용
	float costToGoal;		// h : 현재 노드부터 목적지까지의 예상비용
	float totalCost;		// f : g + h

	AstarTile* parentTile;	// g가 갱신될 때마다 이전 노드를 갱신

	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;

public:
	virtual HRESULT Init();
	HRESULT Init(int idX, int idY);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetColor(COLORREF color);
	void SetType(AstarTileType type) { this->type = type; }
	AstarTileType GetType() { return this->type; }

	virtual ~AstarTile() {};

	friend class AstarScene;

};

class AstarScene : public GameObject
{
	// 이차원 배열 맵을 구성
	AstarTile map[ASTAR_TILE_COUNT][ASTAR_TILE_COUNT];

	AstarTile* startTile;	// 빨간색
	AstarTile* destTile;	// 파란색

	AstarTile* currTile;	// (녹색) 후보타일을 선정할 때의 기준타일

	vector<AstarTile*> openList;
	vector<AstarTile*> closeList;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void FindPath();
	void AddOpenList(AstarTile* currTile);
	void UpdateNeighborCosts(AstarTile* neighbor, AstarTile* current);
	float Heuristic(AstarTile* a, AstarTile* b);
	AstarTile* GetLowestCostTile();
	void PrintPath();
	bool IsValidNeighbor(AstarTile* neighbor);

	virtual ~AstarScene() {};
};

