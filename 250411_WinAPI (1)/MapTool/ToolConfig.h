#pragma once

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <wrl/client.h>  // ComPtr

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

static unsigned int g_ResizeWidth = 0, g_ResizeHeight = 0;