#pragma once
#include "Singleton.h"

class Tile;
//게임 매니저는 각씬마다 있어야할까?

//위가 비어있는 타일 위에 생성된다.
struct JumpNode
{
	int x, y;      // (x, y)
	FPOINT worldPos;     // 실제 위치 (중앙)
	vector<JumpNode*> neighbors;

	// A*에 필요한 정보
	float gCost = 0.f;  // 시작부터 여기까지 실제 비용
	float hCost = 0.f;  // 목표까지의 휴리스틱
	float fCost() const { return gCost + hCost; }

	JumpNode* parent = nullptr;
	bool visited = false;
};

//일단 각씬마다 있는형태.
class GameManager : public Singleton<GameManager>
{
public:
	void Release();
	void ReleaseStage();

	void LoadTile(const char* path, bool isCave = false);
	void LoadObject(const char* path, bool isCave = false);
	void GenerateBorderTile();
	void GenerateDecoTile();

	void GenerateDecoTile(int tileX, int tileY);
	void UpdateAdjacentDecoTiles(int tileX, int tileY);
	void CreateCaveRendertarget(ID2D1RenderTarget* mainRT);
	void Init(const char* path);
	void Init();
	void GenerateCave(const char* path);
	ID2D1BitmapRenderTarget* GetCaveRenderTarget();

	void DestructionTile(const FPOINT& tilePos);
	bool HasTile(int x, int y);
	void SetPlayerImageKey(string key);
	static vector<JumpNode*> jumpNodes;
	vector<JumpNode*> FindPath(JumpNode* start, JumpNode* goal);

	JumpNode* FindClosestJumpNode(const FPOINT& worldPos);

	void UpdateNavMesh();

	void TravelToNextScene();

	void LoadScenes(const char* sceneName);
private:
	bool IsTileValid(int x, int y, bool isCave = false);
	void BuildJumpNodesFromTileMap();
	void LinkWalkableNeighbors();
	bool IsWalkableBetween(int x1, int y1, int x2, int y2);
	void LinkFallNeighbors();
	void LinkJumpableNeighbors();
	bool IsJumpPathClear(FPOINT from, FPOINT to);
	JumpNode* FindJumpNodeAt(int x, int y);
	IPOINT WorldToTile(float x, float y);
	bool IsStandableTile(int x, int y);
	float Heuristic(const FPOINT& a, const FPOINT& b)
	{
		float dx = b.x - a.x;
		float dy = b.y - a.y;
		return sqrtf(dx * dx + dy * dy);  // 유클리드 거리
	}
private:
	Tile* tileMap[36][44];
	Tile* caveTileMap[36][44];
	ComPtr<ID2D1BitmapRenderTarget> caveRenderTarget;
	string playerImageKey;
	
	vector<string> stageInfos;
	int currentStateIndex = 0;

	bool isTileChanged = false;
	
};

