﻿#include <tchar.h>
#include <iostream>
#include "Framework/Common/MemoryManager.h"
#include "Framework/Common/AssetLoader.h"
#include "Framework/Common/utility.h"
#include "Framework/Parser/BmpParser.h"
#include "Framework/Platform/WindowsApplication.h"
#include "Framework/RHI/D2DGraphicsManager.h"

using namespace std;
using namespace Engine;

namespace Engine {

	class TestGraphicsManager : public D2DGraphicsManager
	{
	private:
		ID2D1Bitmap* m_pBitmap = nullptr;

	public:
		void DrawBitmap(const Image *image, int32_t index);
	};

	class TestApplication : public WindowsApplication
	{
	private:
		Image m_Image[2];

	public:
		TestApplication(GfxConfiguration& config) : WindowsApplication(config) {};
		virtual int Initialize();
		virtual void OnDraw();
	};

}

namespace Engine {

	GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 1024, 512, _T("Texture Load Test (Windows)"));
	IApplication* g_pApp = static_cast<IApplication*>(new TestApplication(config));
	GraphicsManager* g_pGraphicsManager = static_cast<GraphicsManager*>(new TestGraphicsManager());
	MemoryManager* g_pMemoryManager = static_cast<MemoryManager*>(new MemoryManager());

}

int Engine::TestApplication::Initialize()
{
	int result;

	result = WindowsApplication::Initialize();

	if (result == 0) {
		AssetLoader asset_loader;
		BmpParser   parser;
		Buffer buf = asset_loader.SyncOpenAndReadBinary("Textures/icelogo-color.bmp");

		m_Image[0] = parser.Parse(buf);

		buf = asset_loader.SyncOpenAndReadBinary("Textures/icelogo-normal.bmp");

		m_Image[1] = parser.Parse(buf);
	}

	return result;
}

void Engine::TestApplication::OnDraw()
{
	dynamic_cast<TestGraphicsManager*>(g_pGraphicsManager)->DrawBitmap(m_Image, 0);
	dynamic_cast<TestGraphicsManager*>(g_pGraphicsManager)->DrawBitmap(m_Image, 1);
}

void Engine::TestGraphicsManager::DrawBitmap(const Image* image, int32_t index)
{
	HRESULT hr;

	// start build GPU draw command
	m_pRenderTarget->BeginDraw();

	D2D1_BITMAP_PROPERTIES props;
	props.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	props.dpiX = 72.0f;
	props.dpiY = 72.0f;
	SafeRelease(&m_pBitmap);
	hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(image[index].Width, image[index].Height),
		image[index].data, image[index].pitch, props, &m_pBitmap);

	D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
	D2D1_SIZE_F bmpSize = m_pBitmap->GetSize();

	D2D1_RECT_F source_rect = D2D1::RectF(
		0,
		0,
		bmpSize.width,
		bmpSize.height
	);

	float aspect = bmpSize.width / bmpSize.height;
	float dest_height = rtSize.height;
	float dest_width = rtSize.height * aspect;

	D2D1_RECT_F dest_rect = D2D1::RectF(
		dest_width * index,
		0,
		dest_width * (index + 1),
		dest_height
	);

	m_pRenderTarget->DrawBitmap(m_pBitmap, dest_rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, source_rect);

	// end GPU draw command building
	m_pRenderTarget->EndDraw();
}


