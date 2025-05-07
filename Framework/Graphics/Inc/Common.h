#pragma once

#include <Core/Inc/Core.h>
#include <Math/Inc/DWMath.h>

//DirectX headers
#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXTK/Inc/CommonStates.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

template<class T>
inline void SafeRelease(T*& ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}