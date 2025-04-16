#pragma once
#include <d3d11.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "config.h"
#include <unordered_map>
#include "EditorTile.h"

constexpr int mapWidth = 16;
constexpr int mapHeight = 16;
class TileMapTool
{
public:
    TileMapTool();
    ~TileMapTool();
    void Init(ID3D11Device* device);             // Ÿ�� ���� �ʱ�ȭ
    void DrawTileMapTool();                 // Ÿ�� �ȷ�Ʈ ���
    int GetSelectedTileID() const;        // ���� ���õ� Ÿ�� ID

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

    float scale = 0.5f;         // Ȯ�� ���� (UI ���� ���ϰ�)
    int atlasWidth = 512;   // �̹��� ���� �ػ� (px)
    int atlasHeight = 896;
    int selectedTileX = -1, selectedTileY = -1;

    float zoom = 0.3f;
    float zoomSpeed = 0.1f;

    ImVec2 cameraOffset = ImVec2(0, 0);

    unordered_map<DecoType, ImTextureID> decoTextures;
    EditorTile tileMap[mapHeight][mapWidth]; // 2D Ÿ�ϸ�
};

