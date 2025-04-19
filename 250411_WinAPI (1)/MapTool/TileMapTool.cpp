#include "TileMapTool.h"
#include <wincodec.h> 
#include <nlohmann/json.hpp>
#pragma comment(lib, "windowscodecs.lib")

using json = nlohmann::json;

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

    for (auto& iter : metaAtlasTexture)
    {
        iter.second->Release();
    }
    metaAtlasTexture.clear();

    ReleaseObjects();
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

    if (ObjectMetaLoader::LoadFromJson("object_meta.json"))
    {
        int a = 10;
        LoadAtlasTexturesFromRegistry(device);
    }
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

void TileMapTool::ReleaseObjects()
{
    for (auto& iter : g_PlacedObjects)
    {
        if (iter)
            delete iter;
    }
    g_PlacedObjects.clear();
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
        SaveTileMapToFile("../250304_WinAPI/Data/hongScene.tilemap");
        SavePlacedObjectToJson("../250304_WinAPI/Data/hongScene.json");
    }
    if (ImGui::Button("Load TileMap"))
    {
        LoadTileMapFromFile("../250304_WinAPI/Data/hongScene.tilemap");
        LoadPlacedObjectsFromJson("../250304_WinAPI/Data/hongScene.json");
    }

    ImGui::End();
}

void TileMapTool::DrawTileMap()
{
    ImGui::Begin("TileMapPreview", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    // 그리드 크기
    ImVec2 mapOrigin = ImGui::GetCursorScreenPos();

    float scaledTileSize = tileSize * zoom;
    ImGui::InvisibleButton("MapDropZone", ImVec2(40 * scaledTileSize, 32 * scaledTileSize));
    ImVec2 screenMouse = ImGui::GetMousePos();

    ImGui::SetCursorScreenPos(mapOrigin);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECT_TYPE"))
        {
            const char* droppedName = static_cast<const char*>(payload->Data);

            // 1. 맵 좌표로 변환 (너 구현에 맞게!)
            FPOINT mapPos = ConvertScreenToMap(screenMouse);  // 너만의 좌표계로!

            // 2. 메타 정보 찾아서 배치
            const ObjectMeta* meta = ObjectRegistry::Get().Find(droppedName);
            if (meta)
            {
                CreateObjectOnMap(meta->name, mapPos);  // 너의 배치 함수!
                printf("[%s] placed at %.1f, %.1f\n", droppedName, mapPos.x, mapPos.y);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // 스크롤 / 드래그 처리
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        cameraOffset.x += io.MouseDelta.x;
        cameraOffset.y += io.MouseDelta.y;
    }
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
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
        selectedTileX >= 0 && selectedTileY >= 0)
    {
        ImVec2 mouse = io.MousePos;
        ImVec2 local = (mouse - origin) / zoom;

        int tx = (int)floorf(local.x / tileSize);
        int ty = (int)floorf(local.y / tileSize);

        if (tx >= 0 && tx < mapWidth && ty >= 0 && ty < mapHeight) {
            tileMap[ty][tx] = { selectedTileX, selectedTileY, true, { (float)tx, (float)ty} };
        }
    }

    int objId = 0;
    for (ToolGameObject* obj : g_PlacedObjects)
    {
        ID3D11ShaderResourceView* texture = metaAtlasTexture[obj->atlas];  // or meta.atlasPath
        if (!texture) continue;

        ImVec2 screenPos = origin + ImVec2(obj->pos.x, obj->pos.y) * zoom;

        // 이미지의 반 크기
        ImVec2 halfSize = ImVec2(obj->width, obj->height) * 0.5f * zoom;

        ImVec2 min = screenPos - halfSize;
        ImVec2 max = screenPos + halfSize;

        // [1] 버튼을 오브젝트 위치에 동적으로 깐다!
        ImVec2 cursorBackup = ImGui::GetCursorScreenPos();  // 커서 위치 저장

        ImGui::SetCursorScreenPos(min);
        std::string id = "##obj_" + std::to_string(objId); // 고유 ID
        ImGui::InvisibleButton(id.c_str(), max - min);
        ImGui::SetCursorScreenPos(cursorBackup);

        // [2] 클릭 판정 → 오브젝트와 연결
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) 
            && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            draggingObject = obj;
            isDragging = true;
            dragOffset = io.MousePos - screenPos;
        }

        ImVec2 atlas = GetTextureSize(texture);

        // UV 계산 (아틀라스)
        float atlasW = atlas.x; // 너가 texture에서 얻어온 크기
        float atlasH = atlas.y;

        ImVec2 uv0 = ImVec2(
            (float)(0 * obj->width) / atlasW,
            (float)(0 * obj->height) / atlasH
        );
        ImVec2 uv1 = ImVec2(
            (float)((0 + 1) * obj->width) / atlasW,
            (float)((0 + 1) * obj->height) / atlasH
        );

        drawList->AddImage((ImTextureID)(uintptr_t)texture, min, max, uv0, uv1);
        
        objId++;
    }

    if (isDragging && draggingObject)
    {
        ImVec2 mouse = io.MousePos;

        // 드래그할 때는 마우스 - dragOffset = 오브젝트 중심 좌표
        ImVec2 center = (mouse - dragOffset - origin) / zoom;

        draggingObject->pos.x = center.x;
        draggingObject->pos.y = center.y;

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            isDragging = false;
            draggingObject = nullptr;
        }
    }

    ImGui::End();
}

void TileMapTool::SaveTileMapToFile(const char* path)
{
    FILE* fp = fopen(path, "wb");
    if (!fp) return;

    fwrite(&tileMap[0][0], sizeof(EditorTile), mapWidth * mapHeight, fp);
    fclose(fp);
}

void TileMapTool::LoadTileMapFromFile(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (!fp) return;

    fread(&tileMap[0][0], sizeof(EditorTile), mapWidth * mapHeight, fp);
    fclose(fp);
}

void TileMapTool::SavePlacedObjectToJson(const char* path)
{
    json j = json::array();

    for (auto& obj : g_PlacedObjects)
    {
        string nameStr = ObjectMetaLoader::WStringToString(obj->name);
        string atlasStr = ObjectMetaLoader::WStringToString(obj->atlas);
        j.push_back({
            { "name", nameStr },
            { "x", obj->pos.x },
            { "y", obj->pos.y },
            { "width", obj->width },
            { "height", obj->height },
            { "atlas", atlasStr }
            });
    }

    std::ofstream out(path);
    out << j.dump(4);
    out.close();

    printf("Saved objects to: %s\n", path);
}

void TileMapTool::LoadPlacedObjectsFromJson(const char* path)
{
    std::ifstream in(path);
    if (!in.is_open())
    {
        printf("Failed to open: %s\n", path);
        return;
    }

    nlohmann::json j;
    in >> j;
    in.close();

    ReleaseObjects();

    for (const auto& item : j)
    {
        ToolGameObject* obj = new ToolGameObject();

        obj->name = ObjectMetaLoader::Utf8ToWstring(item.value("name", ""));
        obj->pos.x = item.value("x", 0.0f);
        obj->pos.y = item.value("y", 0.0f);
        obj->width = item.value("width", 0.0f);
        obj->height = item.value("height", 0.0f);
        obj->atlas = ObjectMetaLoader::Utf8ToWstring(item.value("atlas", ""));;

        g_PlacedObjects.push_back(obj);
    }

    printf("Loaded %zu objects from: %s\n", g_PlacedObjects.size(), path);
}

void TileMapTool::LoadAtlasTexturesFromRegistry(ID3D11Device* device)
{
    for (const auto& pair : ObjectRegistry::Get().GetAll())
    {
        const ObjectMeta& meta = pair.second;
        
        if (metaAtlasTexture.find(meta.atlasPath) != metaAtlasTexture.end())
            continue;

        ID3D11ShaderResourceView* srv = nullptr;

        wstring atlasPath = L"../250304_WinAPI/" + meta.atlasPath;
        LoadTextureFromFile_WIC(atlasPath.c_str(), device, &srv);
        metaAtlasTexture[meta.atlasPath] = srv;
    }
}

ImVec2 TileMapTool::GetTextureSize(ID3D11ShaderResourceView* srv)
{
    ID3D11Resource* resource = nullptr;
    srv->GetResource(&resource);

    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);

    ImVec2 size = ImVec2(1, 1); // fallback

    if (SUCCEEDED(hr) && texture)
    {
        D3D11_TEXTURE2D_DESC desc;
        texture->GetDesc(&desc);
        size = ImVec2((float)desc.Width, (float)desc.Height);
        texture->Release();
    }

    resource->Release();
    return size;
}

FPOINT TileMapTool::ConvertScreenToMap(ImVec2 screenPos)
{
    //ImVec2 canvasTopLeft = ImGui::GetCursorScreenPos();  // 맵 그리는 영역의 좌상단
    //float tileSize = 64.0f; // 또는 meta->cellSize.x

    //float mapX = (screenPos.x - canvasTopLeft.x);
    //float mapY = (screenPos.y - canvasTopLeft.y);

    //return FPOINT{ mapX, mapY };

    ImVec2 topLeft = ImGui::GetCursorScreenPos();
    ImVec2 relative = (screenPos - topLeft) / zoom;
    relative.x -= cameraOffset.x;
    relative.y -= cameraOffset.y;
    return FPOINT{ relative.x, relative.y };
}

void TileMapTool::CreateObjectOnMap(const std::string& name, FPOINT pos)
{
    const ObjectMeta* meta = ObjectRegistry::Get().Find(name);
    if (!meta) return;

    ToolGameObject* obj = new ToolGameObject();
    obj->name = ObjectMetaLoader::Utf8ToWstring(name);
    obj->pos = pos;
    obj->width = meta->cellWidth;
    obj->height = meta->cellHeight;
    obj->atlas = meta->atlasPath;

    g_PlacedObjects.push_back(obj);
}

void TileMapTool::DrawObjectPalette()
{
    const auto& metas = ObjectRegistry::Get().GetAll();

    ImGui::Begin("Object Palette");

    for (const auto& pair : ObjectRegistry::Get().GetAll())
    {
        const std::string& name = pair.first;
        const ObjectMeta& meta = pair.second;

        ID3D11ShaderResourceView* texture = metaAtlasTexture[meta.atlasPath];
        if (!texture)
            continue;

        ImVec2 atlasSize = GetTextureSize(texture); // ← atlas 텍스처의 실제 크기

        // UV 좌표 전체 텍스처 기준 (0~1)
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);
        ImVec2 uv1 = ImVec2((float)meta.cellWidth / atlasSize.x, (float)meta.cellHeight / atlasSize.y); // ← 2048 고정 해상도 기준

        ImGui::BeginGroup();

        ImTextureID textureId = reinterpret_cast<uintptr_t>(texture);
        if(ImGui::ImageButton(name.c_str(), textureId, ImVec2(64, 64), uv0, uv1))
        {
            selectedObjectName = name;
        }

        // 드래그 시작
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("OBJECT_TYPE", name.c_str(), name.length() + 1);
            ImGui::Text("%s", name.c_str());
            ImGui::EndDragDropSource();
        }

        ImGui::Text("%s", name.c_str());

        ImGui::EndGroup();

        ImGui::SameLine();
    }
    ImGui::End();
}