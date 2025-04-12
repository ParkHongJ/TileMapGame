#pragma once
#include <d3d11.h>
#include "imgui.h"
#include "config.h"


class TileMapTool
{
public:
    TileMapTool();
    ~TileMapTool();
    void Init(ID3D11Device* device);             // Ÿ�� ���� �ʱ�ȭ
    void DrawPaletteUI();                 // Ÿ�� �ȷ�Ʈ ���
    int GetSelectedTileID() const;        // ���� ���õ� Ÿ�� ID

    ImTextureID LoadTextureFromFile_WIC(const wchar_t* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv);
private:
    int selectedTileID = 0;
    std::vector<ImTextureID> tileTextures;
    std::vector<ID3D11ShaderResourceView*> shaderViews;
    int tilesPerRow = 4;
    ImVec2 tileSize = ImVec2(48, 48);
};

