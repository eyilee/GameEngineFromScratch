#pragma once

#include <d3d11.h>
#include <d3d11_1.h>
#include <cstdint>
#include "Framework/Common/Buffer.h"
#include "Framework/Common/GraphicsManager.h"
#include "Framework/Common/Image.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace Engine {

	// vertex buffer structure
	struct VERTEX {
		XMFLOAT3    Position;
		XMFLOAT4    Color;
	};

	class D3D11GraphicsManager : public GraphicsManager
	{
	public:
		virtual int Initialize();
		virtual void Finalize();

		virtual void Tick();

		virtual void DrawSingleMesh(const Mesh& mesh);

	private:
		void CreateRenderTarget();
		void SetViewPort();
		void InitPipeline();
		void InitGraphics();
		HRESULT CreateGraphicsResources();
		void DiscardGraphicsResources();
		void RenderFrame();

	private:
		const uint32_t SCREEN_WIDTH = 960;
		const uint32_t SCREEN_HEIGHT = 480;

		IDXGISwapChain          *g_pSwapchain = nullptr;              // the pointer to the swap chain interface
		ID3D11Device            *g_pDev = nullptr;              // the pointer to our Direct3D device interface
		ID3D11DeviceContext     *g_pDevcon = nullptr;              // the pointer to our Direct3D device context

		ID3D11RenderTargetView  *g_pRTView = nullptr;

		ID3D11InputLayout       *g_pLayout = nullptr;              // the pointer to the input layout
		ID3D11VertexShader      *g_pVS = nullptr;              // the pointer to the vertex shader
		ID3D11PixelShader       *g_pPS = nullptr;              // the pointer to the pixel shader

		ID3D11Buffer            *g_pVBuffer = nullptr;
	};
}
