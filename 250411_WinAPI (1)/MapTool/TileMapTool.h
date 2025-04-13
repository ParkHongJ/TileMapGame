#pragma once
#include <d3d11.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "config.h"
#include <unordered_map>

enum class DecoType {
    None, A, B, C, D
};

struct Deco {
    DecoType type;
    bool flipH = false;
};

struct Tile {
    int atlasX = -1; // Atlas 내 타일 좌표
    int atlasY = -1;
    bool valid = false;

    std::vector<Deco> decos; // 이 타일에 붙는 데코 목록
};

constexpr int mapWidth = 16;
constexpr int mapHeight = 16;
class TileMapTool
{
public:
    TileMapTool();
    ~TileMapTool();
    void Init(ID3D11Device* device);             // 타일 개수 초기화
    void DrawTileMapTool();                 // 타일 팔레트 출력
    int GetSelectedTileID() const;        // 현재 선택된 타일 ID

    ImTextureID LoadTextureFromFile_WIC(const wchar_t* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv);
private:
    void DrawPaletteUI();
    void DrawTileMap();

    void SaveTileMapToFile(const char* path);
    void LoadTileMapFromFile(const char* path);
    void UpdateTileDecos();

private:
    int selectedTileID = 0;

    std::vector<ImTextureID> tileTextures;
    std::vector<ID3D11ShaderResourceView*> shaderViews;

    const int tileSize = 128;

    float scale = 0.5f;         // 확대 배율 (UI 보기 편하게)
    int atlasWidth = 512;   // 이미지 원본 해상도 (px)
    int atlasHeight = 896;
    int selectedTileX = -1, selectedTileY = -1;

    float zoom = 0.3f;
    float zoomSpeed = 0.1f;

    ImVec2 cameraOffset = ImVec2(0, 0);

    unordered_map<DecoType, ImTextureID> decoTextures;
    Tile tileMap[mapHeight][mapWidth]; // 2D 타일맵
};

