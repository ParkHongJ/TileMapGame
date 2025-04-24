#pragma once
#include "Singleton.h"

class Tile;
//���� �Ŵ����� �������� �־���ұ�?

//���� ����ִ� Ÿ�� ���� �����ȴ�.
struct JumpNode
{
	int x, y;      // (x, y)
	FPOINT worldPos;     // ���� ��ġ (�߾�)
	vector<JumpNode*> neighbors;

	// A*�� �ʿ��� ����
	float gCost = 0.f;  // ���ۺ��� ������� ���� ���
	float hCost = 0.f;  // ��ǥ������ �޸���ƽ
	float fCost() const { return gCost + hCost; }

	JumpNode* parent = nullptr;
	bool visited = false;
};

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
		return sqrtf(dx * dx + dy * dy);  // ��Ŭ���� �Ÿ�
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

