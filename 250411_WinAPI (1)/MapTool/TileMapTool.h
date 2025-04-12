#pragma once
#include <d3d11.h>
#include "imgui.h"
#include "config.h"


class TileMapTool
{
public:
    TileMapTool();
    ~TileMapTool();
    void Init(ID3D11Device* device);             // 타일 개수 초기화
    void DrawPaletteUI();                 // 타일 팔레트 출력
    int GetSelectedTileID() const;        // 현재 선택된 타일 ID

    ImTextureID LoadTextureFromFile_WIC(const wchar_t* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv);
private:
    int selectedTileID = 0;
    std::vector<ImTextureID> tileTextures;
    std::vector<ID3D11ShaderResourceView*> shaderViews;
    int tilesPerRow = 4;
    ImVec2 tileSize = ImVec2(48, 48);
};

