#include "TileMapTool.h"
#include <wincodec.h> 
#pragma comment(lib, "windowscodecs.lib")

TileMapTool::TileMapTool()
{
}

TileMapTool::~TileMapTool()
{
	for (auto* srv : shaderViews)
	{
		if (srv)
		{
			srv->Release();
			srv = nullptr;
		}
	}

	shaderViews.clear();
	tileTextures.clear();
}

void TileMapTool::Init(ID3D11Device* device)
{
    /*for (int i = 0; i < tileCount; ++i)
    {
        std::wstring path = L"tiles/tile_" + std::to_wstring(i) + L".png";
        ID3D11ShaderResourceView* srv = nullptr;

        ImTextureID tex = LoadTextureFromFile_WIC(path.c_str(), device, &srv);

        tileTextures[i] = tex;
        shaderViews[i] = srv;
    }*/

    wstring path = L"../250304_WinAPI/Textures/CaveTile.png";
    ID3D11ShaderResourceView* srv = nullptr;

    ImTextureID tex = LoadTextureFromFile_WIC(path.c_str(), device, &srv);

    tileTextures.push_back(tex);
    shaderViews.push_back(srv);
}

void TileMapTool::DrawTileMapTool()
{
    DrawPaletteUI();
    DrawTileMap();
}

int TileMapTool::GetSelectedTileID() const
{
    return selectedTileID;
}

ImTextureID TileMapTool::LoadTextureFromFile_WIC(const wchar_t* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv)
{
    IWICImagingFactory* wicFactory = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    HRESULT hr = CoInitialize(nullptr); // COM 초기화
    if (FAILED(hr)) return (ImTextureID)nullptr;

    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&wicFactory));
    if (FAILED(hr)) return (ImTextureID)nullptr;

    hr = wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ,
        WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) return (ImTextureID)nullptr;

    decoder->GetFrame(0, &frame);

    wicFactory->CreateFormatConverter(&converter);
    converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);

    UINT width, height;
    converter->GetSize(&width, &height);

    std::vector<BYTE> imageData(width * height * 4);
    converter->CopyPixels(nullptr, width * 4, static_cast<UINT>(imageData.size()), imageData.data());

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData.data();
    initData.SysMemPitch = width * 4;

    ID3D11Texture2D* tex = nullptr;
    device->CreateTexture2D(&desc, &initData, &tex);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    ID3D11ShaderResourceView* srv = nullptr;
    device->CreateShaderResourceView(tex, &srvDesc, &srv);
    tex->Release();

    frame->Release();
    converter->Release();
    decoder->Release();
    wicFactory->Release();
    CoUninitialize();

    *out_srv = srv;
    return (ImTextureID)srv;
}

void TileMapTool::DrawPaletteUI()
{
    ImGui::Begin("Tile Palette");

    // 1. Atlas 이미지 출력 위치 계산
    ImVec2 atlasDisplaySize = ImVec2(atlasWidth * scale, atlasHeight * scale); // 확대해서 보기
    ImVec2 atlasScreenPos = ImGui::GetCursorScreenPos(); // Atlas가 그려질 실제 화면 좌표
    ImGui::Image(tileTextures[0], atlasDisplaySize);

    // 2. 마우스로 Atlas 클릭 시 타일 선택
    ImVec2 mousePos = ImGui::GetMousePos();
    bool isHovered = ImGui::IsItemHovered();
    bool isClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    if (isHovered && isClicked)
    {
        ImVec2 localPos = ImVec2(
            (mousePos.x - atlasScreenPos.x) / atlasDisplaySize.x * atlasWidth,
            (mousePos.y - atlasScreenPos.y) / atlasDisplaySize.y * atlasHeight
        );

        selectedTileX = (int)(localPos.x / tileSize);
        selectedTileY = (int)(localPos.y / tileSize);
    }

    // 3. Atlas 위에 선택된 타일 테두리 그리기 (★ 여기!)
    if (selectedTileX >= 0 && selectedTileY >= 0)
    {
        float scaleX = atlasDisplaySize.x / (float)atlasWidth;
        float scaleY = atlasDisplaySize.y / (float)atlasHeight;

        ImVec2 tileMin = ImVec2(
            atlasScreenPos.x + selectedTileX * tileSize * scaleX,
            atlasScreenPos.y + selectedTileY * tileSize * scaleY
        );
        ImVec2 tileMax = ImVec2(
            tileMin.x + tileSize * scaleX,
            tileMin.y + tileSize * scaleY
        );

        ImGui::GetWindowDrawList()->AddRect(
            tileMin, tileMax,
            IM_COL32(255, 255, 0, 255), 0.0f, 0, 2.0f
        );
    }

    if (ImGui::Button("Save TileMap"))
    {
        SaveTileMapToFile("map1.tilemap");
    }
    if (ImGui::Button("Load TileMap"))
    {
        LoadTileMapFromFile("map1.tilemap");
    }

    ImGui::End();
}

void TileMapTool::DrawTileMap()
{
    ImGui::Begin("TileMapPreview", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    // 그리드 크기
    ImVec2 mapOrigin = ImGui::GetCursorScreenPos();

    // 스크롤 / 드래그 처리
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        cameraOffset.x += io.MouseDelta.x;
        cameraOffset.y += io.MouseDelta.y;
    }
    float zoomSpeed = 0.1f;
    if (ImGui::IsWindowHovered()) {
        if (io.MouseWheel > 0) zoom += zoomSpeed;
        if (io.MouseWheel < 0) zoom -= zoomSpeed;

        // 최소/최대 줌 제한
        if (zoom >= 5.f)
        {
            zoom = 5.f;
        }
        if (zoom <= 0.25f)
        {
            zoom = 0.25f;
        }
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowOrigin = ImGui::GetCursorScreenPos();
    ImVec2 origin = windowOrigin + cameraOffset;

    // 타일맵 그리기
    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            ImVec2 tileMin = origin + ImVec2(x * tileSize, y * tileSize) * zoom;
            ImVec2 tileMax = tileMin + ImVec2(tileSize, tileSize) * zoom;

            drawList->AddRectFilled(tileMin, tileMax, IM_COL32(50, 50, 50, 255));
            drawList->AddRect(tileMin, tileMax, IM_COL32(100, 100, 100, 255)); // 그리드

            if (tileMap[y][x].valid)
            {
                int tileX = tileMap[y][x].atlasX;
                int tileY = tileMap[y][x].atlasY;

                ImVec2 uv0 = ImVec2((float)(tileX * tileSize) / atlasWidth,
                    (float)(tileY * tileSize) / atlasHeight);
                ImVec2 uv1 = ImVec2((float)((tileX + 1) * tileSize) / atlasWidth,
                    (float)((tileY + 1) * tileSize) / atlasHeight);

                drawList->AddImage(tileTextures[0], tileMin, tileMax, uv0, uv1);
            }
        }
    }

    // 클릭해서 타일 배치
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImVec2 mouse = io.MousePos;
        ImVec2 local = (mouse - origin) / zoom;

        int tx = (int)floorf(local.x / tileSize);
        int ty = (int)floorf(local.y / tileSize);

        if (tx >= 0 && tx < mapWidth && ty >= 0 && ty < mapHeight) {
            tileMap[ty][tx] = { selectedTileX, selectedTileY, true };
        }
    }

    ImGui::End();
}

void TileMapTool::SaveTileMapToFile(const char* path)
{
    FILE* fp = fopen(path, "wb");
    if (!fp) return;

    fwrite(&tileMap[0][0], sizeof(Tile), mapWidth * mapHeight, fp);
    fclose(fp);
}

void TileMapTool::LoadTileMapFromFile(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (!fp) return;

    fread(&tileMap[0][0], sizeof(Tile), mapWidth * mapHeight, fp);
    fclose(fp);
}
