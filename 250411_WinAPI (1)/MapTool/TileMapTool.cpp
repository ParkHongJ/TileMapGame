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

    wstring path = L"back.png";
    ID3D11ShaderResourceView* srv = nullptr;

    ImTextureID tex = LoadTextureFromFile_WIC(path.c_str(), device, &srv);

    tileTextures.push_back(tex);
    shaderViews.push_back(srv);
}

void TileMapTool::DrawPaletteUI()
{
    ImGui::Begin("Tile Palette");

    for (int i = 0; i < tileTextures.size(); ++i)
    {
        if (i > 0 && i % tilesPerRow != 0)
            ImGui::SameLine();

        if (tileTextures[i])
        {
            if (ImGui::ImageButton(("tile" + to_string(i)).c_str(), tileTextures[i], tileSize))
                selectedTileID = i;
        }
        else
        {
            if (ImGui::Button(to_string(i).c_str(), tileSize))
                selectedTileID = i;
        }

        // 선택 테두리
        if (selectedTileID == i)
        {
            ImGui::GetWindowDrawList()->AddRect(
                ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                IM_COL32(255, 255, 0, 255), 0.0f, 0, 2.0f
            );
        }
    }

    ImGui::Text("Selected Tile: %d", selectedTileID);
    ImGui::End();
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
