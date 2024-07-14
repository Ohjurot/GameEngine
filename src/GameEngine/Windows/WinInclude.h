#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>

#ifdef ENGINE_DEBUG
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#endif

#include <GameEngine/Windows/ComPointer.h>
