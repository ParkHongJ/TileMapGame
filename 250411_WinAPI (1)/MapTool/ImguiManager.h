#pragma once
#include "config.h"
#include "ToolConfig.h"
class ImGuiManager
{
public:
    bool Init(HWND hwnd, UINT width, UINT height);
    void Shutdown();

    void Begin();
    void End();

    void RenderDockspace();
    void DrawUI();

private:
    HWND m_hwnd = nullptr;
    UINT m_width = 0;
    UINT m_height = 0;

    // DX11 °ü·Ã ¸â¹ö
    ComPtr<ID3D11Device>           m_device;
    ComPtr<ID3D11DeviceContext>    m_context;
    ComPtr<IDXGISwapChain>         m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool InitD3D(HWND hwnd, UINT width, UINT height);
    void CleanupRenderTarget();
    bool CreateRenderTarget();
};