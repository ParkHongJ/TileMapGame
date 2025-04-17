#pragma once
#include <d3d11.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "config.h"
#include <unordered_map>
#include "EditorTile.h"
#include "ObjectRegistry.h"

constexpr int mapWidth = 16;
constexpr int mapHeight = 16;

struct ToolGameObject
{
    std::wstring name;
    FPOINT pos;
    float width;
    float height;
    std::wstring atlas;
};
class TileMapTool
{
public:
    TileMapTool();
    ~TileMapTool();
    void Init(ID3D11Device* device);             // 타일 개수 초기화
    void DrawTileMapTool();                 // 타일 팔레트 출력
    void DrawObjectPalette();
    int GetSelectedTileID() const;        // 현재 선택된 타일 ID

    ImTextureID LoadTextureFromFile_WIC(const wchar_t* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv);

private:
    void ReleaseObjects();
    void DrawPaletteUI();
    void DrawTileMap();

    void SaveTileMapToFile(const char* path);
    void LoadTileMapFromFile(const char* path);
    void SavePlacedObjectToJson(const char* path);
    void LoadPlacedObjectsFromJson(const char* path);

    void LoadAtlasTexturesFromRegistry(ID3D11Device* device);
    ImVec2 GetTextureSize(ID3D11ShaderResourceView* srv);
    FPOINT ConvertScreenToMap(ImVec2 screenPos);

    void CreateObjectOnMap(const std::string& name, FPOINT pos);
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
    EditorTile tileMap[mapHeight][mapWidth]; // 2D 타일맵

    std::string selectedObjectName;
    std::unordered_map<std::wstring, ID3D11ShaderResourceView*> metaAtlasTexture;
    std::vector<ToolGameObject*> g_PlacedObjects;

    ToolGameObject* draggingObject = nullptr;
    bool isDragging = false;
    ImVec2 dragOffset;
};

